#pragma once
#include <pp_common/base.hpp>
#include <string>
#include <unordered_map>

struct xGlobaConfig : public xel::xConfigLoader {

	void Load(const char * filename);

	xel::xNetAddress BindAddress;
	std::string      TerminalKey;

	std::string Mysql_Address;  // host or ip:port
	std::string Mysql_User;
	std::string Mysql_Password;
	std::string Mysql_Schema;

	//
	uint64_t                                  LastUpdateTimestampMS = xel::GetTimestampMS();
	std::unordered_map<uint32_t, std::string> VersionKeys           = {};

	std::string BackendServerListFilename;
};

extern xGlobaConfig GlobalConfig;
