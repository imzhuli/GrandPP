#include "./config_center_client.hpp"

#include <core/core_time.hpp>

using namespace xel;

bool xCCClient::Init(xel::xIoContext * ICP, const xel::xNetAddress & TargetAddress) {
	assert(!Ready);
	RuntimeAssert(xClient::Init(ICP, TargetAddress));
	return true;
}

void xCCClient::Clean() {
	xClient::Clean();
	Reset(Connected);
	Reset(Ready);
}

void xCCClient::Tick(uint64_t NowMS) {
	if (Connected && !Ready && NowMS - LastConnectedTimestamp >= 3'000) {
		Kill();
	}
	xClient::Tick(NowMS);
}

void xCCClient::OnServerConnected() {
	X_DEBUG_PRINTF("");
	assert(!Ready);
	Connected              = true;
	LastConnectedTimestamp = GetTimestampMS();
	PostChallengeRequest();
}

void xCCClient::OnServerClose() {
}

void xCCClient::OnOpenConnection() {
	X_DEBUG_PRINTF("");
}

void xCCClient::OnCleanupConnection() {
	X_DEBUG_PRINTF("");
	LastConnectedTimestamp = 0;
	Connected              = false;
	Ready                  = false;
	OnCCClose();
}

bool xCCClient::OnPacket(const xel::xPacketHeader & Header, xel::ubyte * PayloadPtr, size_t PayloadSize) {
	X_DEBUG_PRINTF("");
	if (Ready) {
		return (Ready = OnCCPacket(Header, PayloadPtr, PayloadSize));
	}
	if ((Ready = OnChallengeResponse(Header, PayloadPtr, PayloadSize))) {
		OnCCReady();
	}
	return true;
}
