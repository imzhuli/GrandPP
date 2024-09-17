#pragma once
#include <pp_common/base.hpp>

struct xConfigServerType {
	static constexpr const uint32_t UNSPEC         = 0;
	static constexpr const uint32_t DNS_DISPATCHER = 1;
	static constexpr const uint32_t DNS_SERVER     = 2;
};

struct xCCConnectionBase {
	uint32_t  Type = xConfigServerType::UNSPEC;
	uint64_t  ConnectionId;
	xServerId ServerId;
};
