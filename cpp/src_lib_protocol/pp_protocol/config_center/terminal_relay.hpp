#pragma once
#include <pp_common/base.hpp>

class xTerminalRelayServerInfo : public xBinaryMessage {
public:
	//

public:
	xel::xIndexId Index = 0;  // index from config center
	std::string   ServerUuid;
	xCountryId    CountryId = 0;
	xNetAddress   ControlAddress;
	xNetAddress   DataAddress;
	//
};

class xGetTerminalServerList : public xBinaryMessage {

public:
	uint64_t ConfigCenterVersion;
	size_t   Total = 0;
};
