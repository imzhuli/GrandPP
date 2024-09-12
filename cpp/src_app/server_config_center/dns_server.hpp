#pragma once
#include <pp_common/base.hpp>

class xDnsServerManager {
public:
	bool Init() {
		return true;
	}
	void Clean() {
	}

public:
	struct DnsServerInfo {
		xGroupId GroupId;
	};

public:
	size_t DnsServerGroups = 0;
};
