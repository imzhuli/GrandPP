#pragma once
#include "../base.hpp"

class xRegisterDnsServer : public xBinaryMessage {
public:
	void SerializeMembers() override {
		W(ForceGroupId, GroupId, ServerId);
	};

	void DeserializeMembers() override {
		R(ForceGroupId, GroupId, ServerId);
	};

	bool      ForceGroupId;
	xGroupId  GroupId;
	xServerId ServerId;
};

class xRegisterDnsServerResp : public xBinaryMessage {
public:
	void SerializeMembers() override {
		W(Accepted, GroupId);
	};

	void DeserializeMembers() override {
		R(Accepted, GroupId);
	};

	bool     Accepted;
	xGroupId GroupId;
};
