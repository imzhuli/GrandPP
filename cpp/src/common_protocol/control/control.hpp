#pragma once
#include "../base.hpp"

class xControlMessage : protected xBinaryMessage {
public:
	xControlMessage() = delete;
	xControlMessage(uint64_t ControlCommandId) {
		this->ControlCommandId = ControlCommandId;
		this->TraceId          = 0;
	}
	void SetTraceId(uint64_t TraceId) {
		this->TraceId = TraceId;
	}

	void SerializeMembers() override {
		W(ControlCommandId, TraceId);
	}
	void DeserializeMembers() override {
		R(ControlCommandId, TraceId);
	}

public:
	uint64_t ControlCommandId;
	uint64_t TraceId;

public:
	constexpr static const uint64_t CMD_REQ_TEST_DELAY        = 0x01;
	constexpr static const uint64_t CMD_REQ_REPORT_TEST_DELAY = 0x02;
	constexpr static const uint64_t CMD_REQ_ECHO              = 0x03;
	constexpr static const uint64_t CMD_REQ_ECHO_RESP         = 0x04;
};

class xControlMessage_RequireTestDelay : xControlMessage {
protected:
	xControlMessage_RequireTestDelay()
		: xControlMessage(CMD_REQ_TEST_DELAY) {
	}
	void SerializeMembers() override {
		xControlMessage::SerializeMembers();
		W(TargetAddress);
	}
	void DeserializeMembers() override {
		xControlMessage::DeserializeMembers();
		R(TargetAddress);
	}

public:
	xNetAddress TargetAddress;
};

class xControlMessage_ReportTestSpeed : xControlMessage {
protected:
	xControlMessage_ReportTestSpeed()
		: xControlMessage(CMD_REQ_REPORT_TEST_DELAY) {
	}
	void SerializeMembers() {
		xControlMessage::SerializeMembers();
		W(TargetAddress, StartTimestampMS, FinishTimestampMS);
	}
	void DeserializeMembers() {
		xControlMessage::DeserializeMembers();
		R(TargetAddress, StartTimestampMS, FinishTimestampMS);
	}

public:
	xNetAddress TargetAddress;
	uint64_t    StartTimestampMS;
	uint64_t    FinishTimestampMS;
};
