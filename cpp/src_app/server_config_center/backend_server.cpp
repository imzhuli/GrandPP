#include "./backend_server.hpp"

#include <core/string.hpp>
#include <filesystem>
#include <fstream>

using namespace xel;
using namespace std;
namespace fs = std::filesystem;

bool xBackendServerManager::Init() {
	RuntimeAssert(IdManager.Init());
	Version = 0;
	return true;
}

void xBackendServerManager::Clean() {
	RemoveAllServers();
	IdManager.Clean();
	Dirty = false;
}

void xBackendServerManager::RemoveAllServers() {
	for (auto & [K, V] : ServerMap) {
		Touch(K);
		delete V;
	}
	ServerMap.clear();
	CachedServerList.clear();
	IdManager.Reset();

	Reset(LastFilepath);
	Reset(LastFileTimesamp);

	Dirty = true;
	Reset(Version);
}

uint32_t xBackendServerManager::AddServer(xNetAddress & Address) {
	auto NewId = IdManager.Acquire();
	if (!NewId) {
		return 0;
	}
	auto ServerInfo     = new xServerInfo;
	ServerInfo->Id      = NewId;
	ServerInfo->Address = Address;

	ServerMap.insert(std::make_pair(NewId, ServerInfo));
	Dirty = true;

	return NewId;
}

void xBackendServerManager::RemoveServer(uint32_t ServerId) {
	assert(ServerId);

	auto Iter = ServerMap.find(ServerId);
	assert(Iter != ServerMap.end());

	IdManager.Release(Iter->first);
	delete (Steal(Iter->second));
	ServerMap.erase(Iter);

	Dirty = true;
}

void xBackendServerManager::RebuildCache() {
	CachedServerList.clear();
	CachedServerList.reserve(ServerMap.size());
	for (auto & [K, V] : ServerMap) {
		Touch(K);
		CachedServerList.push_back(V);
	}
	Dirty = false;
	++Version;
}

bool xBackendServerManager::ReloadFromFile(const char * Filename) {

	auto Error          = std::error_code();
	bool HasFile        = fs::exists(Filename) && fs::is_regular_file(Filename);
	auto NativeFiletime = fs::last_write_time(Filename, Error);
	if (Error) {
		return false;
	}
	if (!HasFile || (Filename == LastFilepath && NativeFiletime <= LastFileTimesamp)) {
		return true;
	}

	auto f = std::ifstream(Filename);
	if (!f) {
		return false;
	}

	RemoveAllServers();
	auto line = std::string();
	while (!f.eof()) {
		std::getline(f, line);
		line = Trim(line);
		if (line.empty()) {
			continue;
		}
		auto Address = xNetAddress::Parse(line);
		if (!Address || !Address.Port) {
			X_PERROR("Invalid address");
		}
		if (!AddServer(Address)) {
			X_PERROR("failed to add more addresses");
		}
	}

	LastFilepath     = Filename;
	LastFileTimesamp = NativeFiletime;
	return true;
}

auto xBackendServerManager::GetServerListCache() -> const std::vector<xServerInfo *> & {
	if (Dirty) {
		RebuildCache();
	}
	return CachedServerList;
}
