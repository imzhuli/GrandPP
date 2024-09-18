#pragma once
#include <core/core_min.hpp>
#include <filesystem>
#include <map>
#include <network/net_address.hpp>
#include <object/object.hpp>
#include <vector>

class xBackendServerManager {
public:
	bool Init();
	void Clean();

	struct xServerInfo {
		uint32_t         Id = 0;
		xel::xNetAddress Address;
	};

public:
	uint32_t AddServer(xel::xNetAddress & Address);
	void     RemoveServer(uint32_t ServerId);
	void     RemoveAllServers();
	void     RebuildCache();
	bool     ReloadFromFile(const char * filename);
	auto     GetServerListCache() -> const std::vector<xServerInfo *> &;

public:
	xel::xObjectIdManagerMini IdManager;
	uint32_t                  Version = 0;
	bool                      Dirty   = false;

	// reload from file:
	std::string                     LastFilepath;
	std::filesystem::file_time_type LastFileTimesamp;

	std::map<uint32_t, xServerInfo *> ServerMap;
	std::vector<xServerInfo *>        CachedServerList;
};
