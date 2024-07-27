#pragma once
#include "../pp_framework/framework.hpp"
#include "../pp_framework/object.hpp"

#include <string>
#include <string_view>

struct xPPHttpProxyOptions {
	xPPThreadContext * ThreadContext;
	std::string_view   UserView;
	std::string_view   PassView;
};

class xPPHttpProxyClient : xPPObject {
public:
	bool Init(xPPHttpProxyOptions & Options);
	void Clean();

public:
	std::string User;
	std::string Pass;
};
