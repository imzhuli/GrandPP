#pragma once
#include "../common/base.hpp"

class xGroupedMessage : public xBinaryMessage {
public:
	void SerializeMembers() override {
		W(GroupId);
	}
	void DeserializeMembers() override {
		R(GroupId);
	}

public:
	uint32_t GroupId = 0;
};
