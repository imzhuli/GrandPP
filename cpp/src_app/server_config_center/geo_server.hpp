#pragma once
#include <pp_common/base.hpp>

class xGeoServerInfo {

public:
	void        AddGeoServer(const xNetAddress & Address);
	void        RemoveGeoServer(const xNetAddress & Address);
	xNetAddress GetGeoServer() const;

private:
	std::vector<xNetAddress> GeoServers;

	//
};
