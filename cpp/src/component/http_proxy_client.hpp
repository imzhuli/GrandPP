#pragma once
#pragma once
#include "../common/base.hpp"

class xHttpProxyClient final : xTcpConnection::iListener {
protected:
	bool Init(xIoContext * ICP, const xNetAddress & ProxyAddress, const std::string & User, const std::string & Pass, const std::string & TargetHost);
	void Clean();

public:
	static xHttpProxyClient * Create(
		xIoContext * ICP, const xNetAddress & ProxyAddress, const std::string & User, const std::string & Pass, const std::string & TargetHost,
		xAllocator * AP = &DefaultAllocator
	);
	static void Destroy(xHttpProxyClient * P, xAllocator * AP = &DefaultAllocator);

protected:
	X_API_MEMBER void   OnConnected(xTcpConnection * TcpConnectionPtr) override;
	X_API_MEMBER void   OnPeerClose(xTcpConnection * TcpConnectionPtr) override;
	X_API_MEMBER size_t OnData(xTcpConnection * TcpConnectionPtr, void * DataPtr, size_t DataSize) override;

private:
	xTcpConnection Conn;
	std::string    AccountName;
	std::string    Password;
	std::string    TargetHost;
};
