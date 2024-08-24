#include "./config_service.hpp"

xNetAddress xConfigService::GetErrorReportServerAddress() {
	if (ErrorReportServers.empty()) {
		return {};
	}
	auto r = RandomGenerator() % ErrorReportServers.size();
	return ErrorReportServers[r].Address;
}

bool xConfigService::Init(xIoContext * PIC, const xNetAddress & LocalAddress) {
	assert(PIC);
	if (!xService::Init(PIC, LocalAddress)) {
		return false;
	}
	ConstructValue(RandomGenerator, RandomDevice());
	return true;
}

void xConfigService::Clean() {
	xService::Clean();
}

void xConfigService::Tick() {
	Ticker.Update();
}

void xConfigService::OnClientConnected(xServiceClientConnection & Connection) {
}

void xConfigService::OnClientClose(xServiceClientConnection & Connection) {
}

bool xConfigService::OnPacket(xServiceClientConnection & Connection, const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) {

	return true;
}