#include "./server_group.hpp"

xServerConnection::xServerConnection(xIoContext * ICP, const xServerInfo & ServerInfo) {
	assert(ICP);
	assert(ServerInfo.Address);
	RuntimeAssert(xClient::Init(ICP, ServerInfo.Address));
}

xServerConnection::~xServerConnection() {
	xClient::Clean();
}

bool xServerConnection::OnPacket(const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) {
	X_DEBUG_PRINTF("");
	return SCGP->OnPacket(Header, PayloadPtr, PayloadSize);
}

xServerConnectionGroup::~xServerConnectionGroup() {
	Todo("Remove clients");
}

void xServerConnectionGroup::Tick(uint64_t NowMS) {
	while (auto SCP = ConnectionList.PopHead([ConnectionTickTime = NowMS - 1'000](const xServerConnection & SC) {
		return SC.GetTickTimeMS() <= ConnectionTickTime;
	})) {
		SCP->Tick(NowMS);
		ConnectionList.AddTail(*SCP);
	}
}
