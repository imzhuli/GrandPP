#pragma once
#include "../base.hpp"

class xControlMessageHeader : public xBinaryMessage {
protected:
	void SerializeMembers() override {
		W(ControlCommand, TraceId);
	}
	void DeserializeMembers() override {
		R(ControlCommand, TraceId);
	}

public:
	uint32_t ControlCommand;
	uint64_t TraceId;

public:
	static constexpr const uint32_t CC_TestDelay     = 0x01;
	static constexpr const uint32_t CC_TestDelayResp = 0x02;
	static constexpr const uint32_t CC_Echo          = 0x03;
	static constexpr const uint32_t CC_EchoResp      = 0x04;
};

class xControlMessage_TestDelay : public xBinaryMessage {
protected:
	void SerializeMembers() override {
		W(TargetAddress);
	}
	void DeserializeMembers() override {
		R(TargetAddress);
	}

public:
	xNetAddress TargetAddress;
};

class xControlMessage_TestSpeedResp : public xBinaryMessage {
protected:
	void SerializeMembers() {
		W(DiffTimestampMS);
	}
	void DeserializeMembers() {
		R(DiffTimestampMS);
	}

public:
	uint64_t DiffTimestampMS;
};
