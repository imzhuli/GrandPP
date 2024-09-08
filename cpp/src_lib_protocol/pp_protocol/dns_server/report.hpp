#pragma once
#include "../base.hpp"

class xRegisterDnsServer : xBinaryMessage {
public:
	void SerializeMembers() override {
		W(GroupId, ServerId);
	};

	void DeserializeMembers() override {
		W(GroupId, ServerId);
	};

	xGroupId  GroupId;
	xServerId ServerId;
};

class xRegisterDnsServerResp : xBinaryMessage {
public:
	void SerializeMembers() override {
		W(Accepted, GroupId);
	};

	void DeserializeMembers() override {
		W(Accepted, GroupId);
	};

	bool     Accepted;
	xGroupId GroupId;
};
