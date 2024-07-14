#include "./http_proxy_client.hpp"

#include <crypto/base64.hpp>
#include <sstream>

xHttpProxyClient * xHttpProxyClient::Create(
	xIoContext * ICP, const xNetAddress & ProxyAddress, const std::string & User, const std::string & Pass, const std::string & TargetHost,
	xHttpProxyClient::iProxyListner * LP, xAllocator * AP
) {
	auto P = ::xel::Create<xHttpProxyClient>(AP);
	if (!P) {
		return nullptr;
	}

	if (!P->Init(ICP, ProxyAddress, User, Pass, TargetHost, LP)) {
		::xel::Destroy<xHttpProxyClient>(AP, P);
		return nullptr;
	}

	return P;
}

void xHttpProxyClient::Destroy(xHttpProxyClient * P, xAllocator * AP) {
	assert(P);
	P->Clean();
	::xel::Destroy<xHttpProxyClient>(AP, P);
}

std::string xHttpProxyClient::MakeAuthString(const std::string & AccountName, const std::string & Password) {
	auto Auth = AccountName + ":" + Password;
	return Base64Encode(Auth.data(), Auth.size());
}

bool xHttpProxyClient::Init(
	xIoContext * ICP, const xNetAddress & ProxyAddress, const std::string & User, const std::string & Pass, const std::string & TargetHost,
	iProxyListner * Listener
) {
	if (!Conn.Init(ICP, ProxyAddress, this)) {
		return false;
	}
	this->TargetHost = TargetHost;
	this->Listener   = Listener;
	this->Stage      = eStage::CONNECTING;

	auto Auth = MakeConnectHeader();
	Conn.PostData(Auth.data(), Auth.size());
	return true;
}

void xHttpProxyClient::Clean() {
	Conn.Clean();
}

void xHttpProxyClient::OnConnected(xTcpConnection * TcpConnectionPtr) {
	Stage = eStage::CONNECTED;
	Listener->OnConnected();
}

void xHttpProxyClient::OnPeerClose(xTcpConnection * TcpConnectionPtr) {
	Stage = eStage::CLOSED;
	Listener->OnClose();
}

static const char ESTABLISH_TAG[] = " 200 Connection Established\r\n\r\n";

size_t xHttpProxyClient::OnData(xTcpConnection * TcpConnectionPtr, void * DataPtr, size_t DataSize) {
	if (Stage == eStage::CONNECTED) {
		// check and test:
		if (std::string_view((const char *)DataPtr, DataSize).find(ESTABLISH_TAG) == (DataSize - SafeLength(ESTABLISH_TAG))) {
			// connected:
			Stage = eStage::AUTHORIZED;
			Listener->OnConnected();
			return DataSize;
		}
		Listener->OnClose();
		return InvalidDataSize;
	}
	if (Stage == eStage::AUTHORIZED) {
		Listener->OnData(DataPtr, DataSize);
		return DataSize;
	}
	return InvalidDataSize;
}

void xHttpProxyClient::PostData(const void * Data, size_t Size) {
	Conn.PostData(Data, Size);
}

std::string xHttpProxyClient::MakeConnectHeader() const {
	auto S = std::ostringstream();
	S << "CONNECT " << TargetHost << "HTTP/1.1\r\n";
	S << "Proxy-Authorization: Basic " << MakeAuth() << "\r\n";
	S << "\r\n";
	return S.str();
}

std::string xHttpProxyClient::MakeAuth() const {
	return MakeAuthString(AccountName, Password);
}
