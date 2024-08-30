#pragma once
#include <pp_common/base.hpp>
#include <string>

struct xConfig {

	xel::xNetAddress BindAddress;
	std::string      TerminalKey;

	std::string Mysql_Address;  // host or ip:port
	std::string Mysql_User;
	std::string Mysql_Password;
	std::string Mysql_Schema;

	//
};
