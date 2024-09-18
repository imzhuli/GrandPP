#pragma once
#include "./dns_dispatcher.hpp"

#include <pp_common/base.hpp>
#include <random>

class xConfigService : xService {
public:
	bool Init(xIoContext * PIC, const xNetAddress & LocalAddress);
	void Clean();
	void Tick();

protected:
	void OnClientConnected(xServiceClientConnection & Connection) override;
	bool OnPacket(xServiceClientConnection & Connection, const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) override;
	void OnCleanupConnection(const xServiceClientConnection & Connection) override;

protected:
	xNetAddress GetErrorReportServerAddress();
	bool        OnEnableDnsDispatcher(xServiceClientConnection & Connection, const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize);

private:
	xTcpServer Server;
	xTicker    Ticker;

	std::vector<xNetAddress> ErrorReportServers;

	xDnsDispatcherManager DnsDispatcherManager;
};
