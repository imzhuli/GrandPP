#pragma once
#include "./connection_base.hpp"

#include <core/core_min.hpp>
#include <core/indexed_storage.hpp>
#include <core/list.hpp>
#include <network/net_address.hpp>
#include <object/object.hpp>

class xDnsDispatcherManager {

	static constexpr const size_t MAX_SERVERS = 1024;

	struct xServerAddress {
		xel::xNetAddress ServiceBindAddress;
		xel::xNetAddress ConsumerBindAddress;
	};

	struct xServerNode
		: xel::xListNode
		, xCCConnectionBase {
		uint64_t         ConnectionId;  // from connection
		uint64_t         ServerId;      // by local id allocator;
		xel::xNetAddress ServiceBindAddress;
		xel::xNetAddress ConsumerBindAddress;
	};

public:
	bool                Init();
	void                Clean();
	void                RemoveServerById(uint64_t ServerId);
	auto                GetAddressList() -> const std::vector<xServerAddress> &;
	xCCConnectionBase * AddServerAddress(const xServerAddress & Address);

private:
	uint32_t                          Version = 0;
	xel::xIndexedStorage<xServerNode> NodeManager;
	xel::xList<xServerNode>           NodeList;

	bool                        Dirty = true;
	std::vector<xServerAddress> CacheAddressList;
	//
};
