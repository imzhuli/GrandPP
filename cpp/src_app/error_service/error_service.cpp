#include "./error_service.hpp"

#include <pp_protocol/error_report.hpp>
#include <pp_protocol/protocol.hpp>

struct xServerAddress {
	xNetAddress Address;
};

bool xErrorService::Init(xIoContext * PIC, const xNetAddress & LocalAddress) {
	assert(PIC);
	if (!xService::Init(PIC, LocalAddress)) {
		return false;
	}
	return false;
}

void xErrorService::Clean() {
	xService::Clean();
}

void xErrorService::Tick() {
	Ticker.Update();
}

void xErrorService::OnClientConnected(xServiceClientConnection & Connection) {
}

void xErrorService::OnClientClose(xServiceClientConnection & Connection) {
}

bool xErrorService::OnPacket(xServiceClientConnection & Connection, const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) {
	switch (Header.CommandId) {
		case Cmd_PostError:
			return OnCmdErrorReport(Connection, Header, PayloadPtr, PayloadSize);
		default:
			break;
	}

	return true;
}

bool xErrorService::OnCmdErrorReport(xServiceClientConnection & Connection, const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) {
	auto Req = xErrorReport();
	if (!Req.Deserialize(PayloadPtr, PayloadSize)) {
		return false;
	}
	cout << Req.ErrorMessage << endl;

	auto Resp     = xErrorReportResp();
	Resp.Accepted = true;

	ubyte Buffer[MaxPacketSize];
	if (auto RSize = WritePacket(Cmd_PostErrorResp, Header.RequestId, Buffer, sizeof(Buffer), Resp)) {
		Connection.PostData(Buffer, RSize);
	}
	return true;
}