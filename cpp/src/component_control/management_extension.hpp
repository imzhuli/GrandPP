#pragma once
#include "../component/base.hpp"

class xManagementExtension
	: xUdpChannel::iListener
	, xNonCopyable {
public:
	bool Init(xIoContext * ICP, xNetAddress & BindAddress);
	void Clean();
	void Tick();

public:
	struct xControlRequest
		: xTimerWheelNode
		, xListNode {
		uint64_t    LocalRequestId;
		uint64_t    LocalTimestampMS;
		uint64_t    SourceRequestId;
		xNetAddress SourceAddress;
		xNetAddress DestinationAddress;
	};

	xControlRequest * CreateRequest();
	void              DestroyRequest(xControlRequest * RP);
	xControlRequest * GetRequestById(uint64_t RequestId);

public:
	void RequestTestDelay(const xNetAddress & Src, const xNetAddress & Dst);

private:
	virtual void OnCommand_TestDelay(void * DataPtr, size_t DataSize, const xControlMessageHeader & Header, const xNetAddress & RemoteAddress);
	virtual void OnCommand_TestDelayResp(void * DataPtr, size_t DataSize, const xControlMessageHeader & Header, const xNetAddress & RemoteAddress);
	virtual void OnCommand_Echo(void * DataPtr, size_t DataSize, const xControlMessageHeader & Header, const xNetAddress & RemoteAddress);
	virtual void OnCommand_EchoResp(void * DataPtr, size_t DataSize, const xControlMessageHeader & Header, const xNetAddress & RemoteAddress);

	void OnData(xUdpChannel * ChannelPtr, void * DataPtr, size_t DataSize, const xNetAddress & RemoteAddress) override;
	void OnRequestTimeout(xControlRequest * RP, uint64_t TimestampMS);

	static void RequestTimeoutCallback(xVariable V, xTimerWheelNode * NP, uint64_t TimestampMS) {
		auto SP = static_cast<xManagementExtension *>(V.P);
		SP->OnRequestTimeout(static_cast<xControlRequest *>(NP), TimestampMS);
	}

private:
	xUdpChannel                      UC;
	xel::xTimerWheel                 TW;
	xIndexedStorage<xControlRequest> RequestPool;
	xList<xControlRequest>           RequestList;

	static constexpr const uint64_t RequestWheelTotal    = 75'00;  // 75s / 10ms
	static constexpr const uint64_t RequestWheelStepMS   = 10;
	static constexpr const uint64_t RequestTimeoutMS     = 2'000;
	static constexpr const uint64_t RequestTimeoutOffset = RequestTimeoutMS / RequestWheelStepMS;
};
