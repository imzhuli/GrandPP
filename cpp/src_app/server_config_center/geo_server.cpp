#include "./geo_server.hpp"

#include <random>

static auto RandomDevice    = std::random_device();
static auto RandomGenerator = std::mt19937(RandomDevice());

void xGeoServerInfo::AddGeoServer(const xNetAddress & Address) {
	GeoServers.push_back(Address);
}

void xGeoServerInfo::RemoveGeoServer(const xNetAddress & Address) {
	// std::erase(GeoServers, Address);
}

xNetAddress xGeoServerInfo::GetGeoServer() const {
	if (GeoServers.empty()) {
		return {};
	}
	auto Distribution = std::uniform_int_distribution<>(0, GeoServers.size() - 1);
	auto Result       = GeoServers[Distribution(RandomGenerator)];

	auto Comp = xNetAddress();
	if (Comp == Result) {
	}

	return Result;
}
