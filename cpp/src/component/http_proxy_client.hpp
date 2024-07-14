#pragma once
#pragma once
#include "../common/base.hpp"

class xHttpProxyClient final : protected xTcpConnection::iListener {
public:
	struct iProxyListner {
		virtual void OnConnected()                        = 0;
		virtual void OnData(void * Data, size_t DataSize) = 0;
		virtual void OnClose()                            = 0;
	};

protected:
	bool Init(
		xIoContext * ICP, const xNetAddress & ProxyAddress, const std::string & User, const std::string & Pass, const std::string & TargetHost,
		iProxyListner * Listener
	);
	void Clean();

public:
	static xHttpProxyClient * Create(
		xIoContext * ICP, const xNetAddress & ProxyAddress, const std::string & User, const std::string & Pass, const std::string & TargetHost,
		iProxyListner * LP, xAllocator * AP = nullptr
	);
	static void        Destroy(xHttpProxyClient * P, xAllocator * AP = nullptr);
	static std::string MakeAuthString(const std::string & AccountName, const std::string & Password);

	void PostData(const void * Data, size_t Size);

protected:
	void   OnConnected(xTcpConnection * TcpConnectionPtr) override;
	void   OnPeerClose(xTcpConnection * TcpConnectionPtr) override;
	size_t OnData(xTcpConnection * TcpConnectionPtr, void * DataPtr, size_t DataSize) override;

private:
	std::string MakeConnectHeader() const;
	std::string MakeAuth() const;

private:
	xTcpConnection  Conn;
	std::string     AccountName;
	std::string     Password;
	std::string     TargetHost;
	iProxyListner * Listener;

	enum struct eStage {
		UNSPEC,
		CONNECTING,
		CONNECTED,
		AUTHORIZED,
		CLOSED,
	};
	eStage Stage = eStage::UNSPEC;
};
