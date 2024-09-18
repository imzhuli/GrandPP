#include "./config_service.hpp"

#include "./config.hpp"

#include <pp_protocol/config_center/dns_dispatcher.hpp>
#include <pp_protocol/protocol.hpp>

xNetAddress xConfigService::GetErrorReportServerAddress() {
	if (ErrorReportServers.empty()) {
		return {};
	}
	auto r = rand() % ErrorReportServers.size();
	return ErrorReportServers[r];
}

bool xConfigService::Init(xIoContext * PIC, const xNetAddress & LocalAddress) {
	assert(PIC);
	RuntimeAssert(xService::Init(PIC, LocalAddress));
	RuntimeAssert(DnsDispatcherManager.Init());
	return true;
}

void xConfigService::Clean() {
	DnsDispatcherManager.Clean();
	xService::Clean();
}

void xConfigService::Tick() {
	Ticker.Update();
	xService::Tick(Ticker);
}

void xConfigService::OnClientConnected(xServiceClientConnection & Connection) {
	auto RemoteAddress = Connection.GetRemoteAddress();
	X_DEBUG_PRINTF("%s", RemoteAddress.ToString().c_str());
}

bool xConfigService::OnPacket(xServiceClientConnection & Connection, const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) {
	switch (Header.CommandId) {
		case Cmd_EnableDnsDispatcher:
			return OnEnableDnsDispatcher(Connection, Header, PayloadPtr, PayloadSize);
	}

	return true;
}

void xConfigService::OnCleanupConnection(const xServiceClientConnection & Connection) {
	auto CP = (xCCConnectionBase *)Connection.GetUserContext().P;
	if (!CP) {
		return;
	}

	X_DEBUG_PRINTF("Type:%u", (unsigned)CP->Type);
	switch (CP->Type) {
		case xConfigServerType::DNS_DISPATCHER:
			return;
		default:
			break;
	}
	return;
}

bool xConfigService::OnEnableDnsDispatcher(xServiceClientConnection & Connection, const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) {
	auto & VP = Connection.GetUserContext().P;
	if (VP) {
		X_DEBUG_PRINTF("invalid challenge");
		return false;
	}

	auto Req = xEnableDnsDispatcher();
	if (PayloadSize != Req.Deserialize(PayloadPtr, PayloadSize)) {
		X_DEBUG_PRINTF("invalid version");
		return false;
	}
	auto Iter = GlobalConfig.VersionKeys.find(Req.Version);
	if (Iter == GlobalConfig.VersionKeys.end()) {
		X_DEBUG_PRINTF("invalid version");
		return false;
	}
	if (Req.Challenge != Req.GenerateChallenge(Req.UnixTimestamp, Req.Version, Iter->second)) {
		X_DEBUG_PRINTF("invalid key");
		return false;
	}

	VP = DnsDispatcherManager.AddServerAddress({ Req.ServiceBindAddress, Req.ConsumerBindAddress });
	if (!VP) {
		X_DEBUG_PRINTF("Failed to add to manager");
		return false;
	}

	auto Resp     = xEnableDnsDispatcherResp();
	Resp.Accepted = true;
	ubyte Buffer[MaxPacketSize];
	auto  RSize = WritePacket(Cmd_EnableDnsDispatcherResp, Header.RequestId, Buffer, sizeof(Buffer), Resp);
	assert(RSize);
	Connection.PostData(Buffer, RSize);
	X_DEBUG_PRINTF("New connection response done");

	return true;
}
