#include "./config.hpp"

xGlobaConfig GlobalConfig;

void xGlobaConfig::Load(const char * filename) {
	Reload(filename);

	Require(BindAddress, "BindAddress");
	Require(TerminalKey, "TerminalKey");

	Require(Mysql_Address, "Mysql_Address");  // host or ip:port
	Require(Mysql_User, "Mysql_User");
	Require(Mysql_Password, "Mysql_Password");
	Require(Mysql_Schema, "Mysql_Schema");
}
