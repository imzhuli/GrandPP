#include "./http_proxy_client.hpp"

#include <crypto/base64.hpp>

bool xPPHttpProxyClient::Init(xPPHttpProxyOptions & Options) {
	auto TC = Options.ThreadContext ? Options.ThreadContext : PPGetMainThreadContext();
	assert(TC);
	assert(Options.ProxyServerHostView.size());
	assert(Options.TargetHostView.size());

	return false;
}

void xPPHttpProxyClient::Clean() {

	return;
}

void xPPHttpProxyClient::GenerateAuthKey(const char * UserStr, const char * PassStr) {
	if (!UserStr) {
		if (!PassStr) {
			AuthKeyHeaderLine = {};
			return;
		} else {
			UserStr = "";
		}
	}
	if (!PassStr) {
		PassStr = "";
	}

	char Buffer[600];
	int  len = snprintf(Buffer, sizeof(Buffer), "%s:%s", UserStr, PassStr);
	if (len <= 0) {
		AuthKeyHeaderLine = {};
		return;
	}
	AuthKeyHeaderLine = "Proxy-Authorization: Basic " + xel::Base64Encode(Buffer, len);
	return;
}
