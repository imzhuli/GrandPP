#include "./http_proxy_client.hpp"

xHttpProxyClient * xHttpProxyClient::Create(
	xIoContext * ICP, const xNetAddress & ProxyAddress, const std::string & User, const std::string & Pass, const std::string & TargetHost, xAllocator * AP
) {
	assert(AP);
	auto P = AP->Create<xHttpProxyClient>();
	if (!P) {
		return nullptr;
	}

	if (!P->Init(ICP, ProxyAddress, User, Pass, TargetHost)) {
		AP->Destroy<xHttpProxyClient>(P);
		return nullptr;
	}

	return P;
}

void xHttpProxyClient::Destroy(xHttpProxyClient * P, xAllocator * AP) {
	assert(AP);
	assert(P);
	P->Clean();
	AP->Destroy<xHttpProxyClient>(P);
}

bool xHttpProxyClient::Init(
	xIoContext * ICP, const xNetAddress & ProxyAddress, const std::string & User, const std::string & Pass, const std::string & TargetHost
) {
	if (!Conn.Init(ICP, ProxyAddress, this)) {
		return false;
	}
	this->TargetHost = TargetHost;
	return true;
}

void xHttpProxyClient::Clean() {
	Conn.Clean();
}
