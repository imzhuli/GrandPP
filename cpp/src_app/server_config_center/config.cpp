#include "./config.hpp"

#include <fstream>
#include <nlohmann_json.hpp>

using JSON = nlohmann::json;
using namespace xel;

xGlobaConfig GlobalConfig;

void xGlobaConfig::Load(const char * filename) {
	Reload(filename);

	Require(BindAddress, "BindAddress");
	Require(TerminalKey, "TerminalKey");

	Require(Mysql_Address, "Mysql_Address");  // host or ip:port
	Require(Mysql_User, "Mysql_User");
	Require(Mysql_Password, "Mysql_Password");
	Require(Mysql_Schema, "Mysql_Schema");

	do {
		std::string Version_Keyfile;
		Require(Version_Keyfile, "Version_Keyfile");
		RuntimeAssert(VersionKeys.empty());

		auto Src = std::ifstream(Version_Keyfile);
		RuntimeAssert(!!Src);

		try {
			auto Data = JSON::parse(Src);
			RuntimeAssert(Data.is_object(), "Require root objec type");

			for (auto & [K, V] : Data.items()) {
				RuntimeAssert(V.is_string(), "Require string type validation key");
				auto Version         = (uint32_t)std::stoul(K, nullptr, 0);
				VersionKeys[Version] = V;
			}

		} catch (std::exception & e) {
			X_PFATAL("%s", e.what());
		}

	} while (false);

	for (auto & [K, V] : VersionKeys) {
		cout << K << ":" << V << endl;
	}
}
