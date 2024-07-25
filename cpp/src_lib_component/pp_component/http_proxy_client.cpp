#include "./http_proxy_client.hpp"

#include <crypto/base64.hpp>
#include <sstream>

bool xHttpProxyClient::Init(xIoContext * ICP, const xProxyOption & Option) {
	return false;
}

void xHttpProxyClient::Clean() {
	assert(Stage != eStage::UNSPEC);
	Stage = eStage::UNSPEC;
}