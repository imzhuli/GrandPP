#pragma once
#include <pp_common/base.hpp>
#include <unordered_map>

struct xRelayServerList {

	enum eTerminalRelayServerGroup : int32_t {
		UNSPEC        = 0,
		ASIA          = -1,
		NORTH_AMERICA = -2,
		SOUTH_AMERICA = -3,
		EUROPE        = -4,
		NORTH_AFRICA  = -5,
		SOUTH_AFRICA  = -6,
		AUSTRALIA     = -7,
	};

	struct xTerminalRelayServerAddress {
		xel::xNetAddress DataAddress;
		xel::xNetAddress ControlAddress;
	};

	struct xTerminalRelayServerInfo : xListNode {
		xServerId                   ServerId;
		xGroupId                    GroupId;
		xTerminalRelayServerAddress Address;
	};

	using xUniquePtr   = std::unique_ptr<xTerminalRelayServerInfo>;
	using xTimeoutList = xel::xList<xTerminalRelayServerInfo>;

	xTimeoutList                           ServerTimeoutList;
	xIndexedStorage<xUniquePtr>            ServerPool;
	std::unordered_map<xGroupId, xIndexId> ServerListByGroup;

	//
};
