#pragma once
#include <pp_common/proxy.hpp>

class xHttpProxyClient final
	: public iProxyBase
	, protected xTcpConnection::iListener {
public:
	bool Init(xIoContext * ICP, const xProxyOption & Option) override;
	void Clean() override;

private:
	xProxyOption    Option;
	xTcpConnection  Conn;
	iProxyListner * Listener;

	enum struct eStage {
		UNSPEC,
		RESOLVING,
		CONNECTING,
		CONNECTED,
		AUTHORIZED,
		CLOSED,
	};
	eStage Stage = eStage::UNSPEC;
};
