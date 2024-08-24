#pragma once
#include <pp_common/base.hpp>
#include <random>

struct xServerAddress {
	xNetAddress Address;
};

class xConfigService : xService {
public:
	bool Init(xIoContext * PIC, const xNetAddress & LocalAddress);
	void Clean();
	void Tick();

protected:
	void OnClientConnected(xServiceClientConnection & Connection) override;
	void OnClientClose(xServiceClientConnection & Connection) override;
	bool OnPacket(xServiceClientConnection & Connection, const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) override;

protected:
	xNetAddress GetErrorReportServerAddress();

private:
	xTcpServer Server;
	xTicker    Ticker;

	std::random_device          RandomDevice;
	std::minstd_rand            RandomGenerator;
	std::vector<xServerAddress> ErrorReportServers;
};
