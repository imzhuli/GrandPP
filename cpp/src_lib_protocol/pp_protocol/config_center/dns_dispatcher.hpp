#pragma once
#include "../base.hpp"

class xGetDnsServerGroupCount : public xBinaryMessage {
public:
	void SerializeMembers() override {
		W(LocalVersion);
	};
	void DeserializeMembers() override {
		R(LocalVersion);
	};

	uint32_t LocalVersion = 0;
};

class xGetDnsServerGroupCountResp : public xBinaryMessage {
public:
	void SerializeMembers() override {
		W(LatestVersion, ServerCount);
	};
	void DeserializeMembers() override {
		R(LatestVersion, ServerCount);
	};

	uint32_t LatestVersion = 0;
	size32_t ServerCount   = 0;
};

class xGetDnsServerList : public xBinaryMessage {
public:
	void SerializeMembers() override {
		W(LocalVersion);
	};

	void DeserializeMembers() override {
		R(LocalVersion);
	};

	uint32_t LocalVersion = 0;
	uint32_t StartIndex   = 0;
};

class xGetDnsServerListResp : public xBinaryMessage {
public:
	void SerializeMembers() override {
		W(LatestVersion);
	};

	void DeserializeMembers() override {
		R(TryAgainTimeoutMS);
	};

	uint32_t                 LatestVersion = 0;
	size32_t                 RemainSize;
	std::vector<xNetAddress> Addresses;
	uint64_t                 TryAgainTimeoutMS = 0;  // if config center is busy updating server list, this value is set to indicate a later retry
};

class xEnableDnsDispatcher : public xBinaryMessage {
public:
	void SerializeMembers() override {
		W(ConsumerBindAddress);
	};

	void DeserializeMembers() override {
		R(ConsumerBindAddress);
	};

	xNetAddress ConsumerBindAddress;
};

class xEnableDnsDispatcherResp : public xBinaryMessage {
public:
	void SerializeMembers() override {
		W(Accepted);
	};

	void DeserializeMembers() override {
		R(Accepted);
	};

	bool Accepted = false;
};
