#include "./management_extension.hpp"

bool xManagementExtension::Init(xIoContext * ICP, xNetAddress & BindAddress) {
	RuntimeAssert(TW.Init(RequestWheelTotal, RequestWheelStepMS));
	RuntimeAssert(RequestPool.Init(1024));
	RuntimeAssert(RequestList.IsEmpty());
	if (!UC.Init(ICP, BindAddress, this)) {
		RequestPool.Clean();
		TW.Clean();
		return false;
	}
	return true;
}

void xManagementExtension::Clean() {
	UC.Clean();
	while (auto RP = RequestList.PopHead()) {
		DestroyRequest(RP);
	}
	assert(RequestList.IsEmpty());
	RequestPool.Clean();
	TW.Clean();
}

void xManagementExtension::Tick() {
	TW.Forward();
}

xManagementExtension::xControlRequest * xManagementExtension::CreateRequest() {
	auto Id = RequestPool.Acquire();
	if (!Id) {
		return nullptr;
	}
	auto RP              = &RequestPool[Id];
	RP->LocalRequestId   = Id;
	RP->LocalTimestampMS = GetTimestampMS();
	TW.ScheduleByOffset(*RP, { &RequestTimeoutCallback, { .P = this } }, RequestTimeoutOffset);
	return RP;
}

void xManagementExtension::DestroyRequest(xControlRequest * RP) {
	X_DEBUG_PRINTF("Id=%" PRIx64 "", RP->LocalRequestId);
	RequestPool.Release(X_DEBUG_STEAL(RP->LocalRequestId));
}

xManagementExtension::xControlRequest * xManagementExtension::GetRequestById(uint64_t RequestId) {
	return RequestPool.CheckAndGet(RequestId);
}

void xManagementExtension::OnData(xUdpChannel *, void * DataPtr, size_t DS, const xNetAddress & RemoteAddress) {
	X_DEBUG_PRINTF("\n%s", HexShow(DataPtr, DS).c_str());
	auto DP       = static_cast<ubyte *>(DataPtr);
	auto Header   = xControlMessageHeader();
	auto Consumed = Header.Deserialize(DP, DS);
	if (!Consumed) {
		X_DEBUG_PRINTF("Invalid header");
		return;
	}
	DP += Consumed;
	DS -= Consumed;
	switch (Header.ControlCommand) {
		case xControlMessageHeader::CC_TestDelay:
			OnCommand_TestDelay(DP, DS, Header, RemoteAddress);
			break;
		case xControlMessageHeader::CC_TestDelayResp:
			OnCommand_TestDelayResp(DP, DS, Header, RemoteAddress);
			break;
		case xControlMessageHeader::CC_Echo:
			OnCommand_Echo(DP, DS, Header, RemoteAddress);
			break;
		case xControlMessageHeader::CC_EchoResp:
			OnCommand_EchoResp(DP, DS, Header, RemoteAddress);
			break;
	}
}

void xManagementExtension::OnRequestTimeout(xControlRequest * RP, uint64_t TimestampMS) {
	DestroyRequest(RP);
}

void xManagementExtension::RequestTestDelay(const xNetAddress & Src, const xNetAddress & Dst) {
	auto RP = CreateRequest();
	if (!RP) {
		return;
	}
	RP->SourceAddress      = Src;
	RP->DestinationAddress = Dst;

	auto ReqHeader           = xControlMessageHeader{};
	ReqHeader.ControlCommand = xControlMessageHeader::CC_TestDelay;
	ReqHeader.TraceId        = RP->LocalRequestId;
	auto ReqBody             = xControlMessage_TestDelay{};
	ReqBody.TargetAddress    = Dst;

	ubyte BB[MaxPacketSize];
	auto  RS = ReqHeader.Serialize(BB, sizeof(BB));
	RS      += ReqBody.Serialize(BB + RS, sizeof(BB) - RS);
	UC.PostData(BB, RS, Src);
}

void xManagementExtension::OnCommand_TestDelay(void * DataPtr, size_t DataSize, const xControlMessageHeader & Header, const xNetAddress & RemoteAddress) {
	X_DEBUG_PRINTF("TraceId: %" PRIx64 "", Header.TraceId);
	auto ReqBody = xControlMessage_TestDelay{};
	if (!ReqBody.Deserialize(DataPtr, DataSize)) {
		X_DEBUG_PRINTF("Invalid data");
		return;
	}

	auto RP = CreateRequest();
	if (!RP) {
		return;
	}
	RP->SourceRequestId    = Header.TraceId;
	RP->SourceAddress      = RemoteAddress;
	RP->DestinationAddress = ReqBody.TargetAddress;

	auto RespHeader           = xControlMessageHeader{};
	RespHeader.ControlCommand = xControlMessageHeader::CC_Echo;
	RespHeader.TraceId        = RP->LocalRequestId;

	ubyte  BB[MaxPacketSize];
	size_t RS = RespHeader.Serialize(BB, sizeof(BB));
	UC.PostData(BB, RS, RP->DestinationAddress);
}

void xManagementExtension::OnCommand_TestDelayResp(void * DataPtr, size_t DataSize, const xControlMessageHeader & Header, const xNetAddress & RemoteAddress) {
	X_DEBUG_PRINTF("TraceId: %" PRIx64 "", Header.TraceId);
	auto Body = xControlMessage_TestSpeedResp{};
	if (!Body.Deserialize(DataPtr, DataSize)) {
		X_DEBUG_PRINTF("Invalid data");
		return;
	}

	auto RP = GetRequestById(Header.TraceId);
	if (!RP) {
		X_DEBUG_PRINTF("Invalid request id");
		return;
	}
	X_DEBUG_PRINTF("%s --> %s Diff: %" PRIx64 "", RP->SourceAddress.ToString().c_str(), RP->DestinationAddress.ToString().c_str(), Body.DiffTimestampMS);
	DestroyRequest(RP);
}

void xManagementExtension::OnCommand_Echo(void * DataPtr, size_t DataSize, const xControlMessageHeader & Header, const xNetAddress & RemoteAddress) {
	X_DEBUG_PRINTF("TraceId: %" PRIx64 "", Header.TraceId);
	auto RespHeader           = xControlMessageHeader{};
	RespHeader.ControlCommand = xControlMessageHeader::CC_EchoResp;
	RespHeader.TraceId        = Header.TraceId;

	ubyte  BB[MaxPacketSize];
	size_t RS = RespHeader.Serialize(BB, sizeof(BB));
	UC.PostData(BB, RS, RemoteAddress);
}

void xManagementExtension::OnCommand_EchoResp(void * DataPtr, size_t DataSize, const xControlMessageHeader & Header, const xNetAddress & RemoteAddress) {
	X_DEBUG_PRINTF("TraceId: %" PRIx64 "", Header.TraceId);
	auto RP = GetRequestById(Header.TraceId);
	if (!RP) {
		X_DEBUG_PRINTF("Invalid request id");
		return;
	}
	auto Diff = GetTimestampMS() - RP->LocalTimestampMS;
	X_DEBUG_PRINTF("Diff: %" PRIx64 "", Diff);

	auto RespHeader           = xControlMessageHeader{};
	RespHeader.ControlCommand = xControlMessageHeader::CC_TestDelayResp;
	RespHeader.TraceId        = RP->SourceRequestId;
	auto RespBody             = xControlMessage_TestSpeedResp{};
	RespBody.DiffTimestampMS  = Diff;

	ubyte  BB[MaxPacketSize];
	size_t RS = RespHeader.Serialize(BB, sizeof(BB));
	RS       += RespBody.Serialize(BB + RS, sizeof(BB) - RS);
	UC.PostData(BB, RS, RP->SourceAddress);
	DestroyRequest(RP);
}
