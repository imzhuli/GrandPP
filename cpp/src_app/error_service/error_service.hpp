#pragma once
#include <pp_common/base.hpp>
#include <random>

class xErrorService : xService {
public:
	bool Init(xIoContext * PIC, const xNetAddress & LocalAddress);
	void Clean();
	void Tick();

protected:
	void OnClientConnected(xServiceClientConnection & Connection) override;
	void OnClientClose(xServiceClientConnection & Connection) override;
	bool OnPacket(xServiceClientConnection & Connection, const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) override;

	bool OnCmdErrorReport(xServiceClientConnection & Connection, const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize);

private:
	xTcpServer Server;
	xTicker    Ticker;
};
