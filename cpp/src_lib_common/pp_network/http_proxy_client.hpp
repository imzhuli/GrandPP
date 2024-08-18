#pragma once
#include "../pp_framework/framework.hpp"
#include "../pp_framework/object.hpp"

#include <network/net_address.hpp>
#include <network/tcp_connection.hpp>
#include <string>
#include <string_view>

struct xPPHttpProxyOptions {
	xPPThreadContext * ThreadContext;
	std::string_view   ProxyServerHostView;
	std::string_view   TargetHostView;
	std::string_view   UserView;
	std::string_view   PassView;
};

class xPPHttpProxyClient
	: xPPObject
	, xel::xTcpConnection::iListener {
public:
	bool Init(xPPHttpProxyOptions & Options);
	void Clean();

protected:
	void GenerateAuthKey(const char * UserStr, const char * PassStr);

public:
	xel::xIoContext *   IoCtx;
	std::string         TargetHost;
	xel::xNetAddress    ProxyServerHostView;
	std::string         AuthKeyHeaderLine;
	xel::xTcpConnection Conn;
};
