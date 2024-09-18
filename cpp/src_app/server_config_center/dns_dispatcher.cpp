#include "./dns_dispatcher.hpp"

using namespace xel;

bool xDnsDispatcherManager::Init() {
	Version = 0;
	NodeManager.Init(MAX_SERVERS);
	Dirty = true;
	return true;
}

void xDnsDispatcherManager::Clean() {
	CacheAddressList.clear();
	NodeManager.Clean();
	return;
}

xCCConnectionBase * xDnsDispatcherManager::AddServerAddress(const xServerAddress & Address) {
	auto NodeId = NodeManager.Acquire();
	if (!NodeId) {
		return nullptr;
	}
	auto & Node              = NodeManager[NodeId];
	Node.Type                = xConfigServerType::DNS_DISPATCHER;
	Node.ServerId            = NodeId;
	Node.ServiceBindAddress  = Address.ServiceBindAddress;
	Node.ConsumerBindAddress = Address.ConsumerBindAddress;
	Dirty                    = true;
	return &Node;
}

void xDnsDispatcherManager::RemoveServerById(uint64_t ServerId) {
	NodeManager.CheckAndRelease(ServerId);
	Dirty = true;
}

auto xDnsDispatcherManager::GetAddressList() -> const std::vector<xServerAddress> & {
	if (Steal(Dirty)) {
		CacheAddressList.clear();
		NodeList.ForEach([&](const xServerNode & Node) { CacheAddressList.push_back({ Node.ServiceBindAddress, Node.ConsumerBindAddress }); });
	}
	return CacheAddressList;
}
