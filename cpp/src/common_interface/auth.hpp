#pragma once
#include "../common/base.hpp"

class iAsyncAuthenticator {
public:
	struct xRequest {
		xVariable        UserContext;
		std::string_view AccountView;
		std::string_view PasswordView;
	};

	enum xResultCode : uint8_t {
		SUCCESS = 0,
		FAILED  = 1,
		TIMEOUT = 2,
	};

	enum xResultType : uint8_t {
		UNSPEC         = 0,
		INTERNAL_RELAY = 1,
		THIRD_HTTP     = 2,
		THIRD_SOCKS5   = 3,
	};

	struct xInternalRelayInfo {
		xNetAddress Entry;
		uint64_t    ClientId;
	};

	struct xThirdHttpInfo {
		xNetAddress      Entry;
		std::string_view HeaderKey;
		std::string_view HeaderValue;
	};

	struct xThirdSocks5Info {
		xNetAddress      Entry;
		std::string_view User;
		std::string_view Password;
	};

	struct xResult {
		xResultCode Code;
		xResultType Type;
		xVariable   UserContext;
		union {
			const xInternalRelayInfo * InternalRelayInfo;
			const xThirdHttpInfo *     ThirdHttpInfo;
			const xThirdSocks5Info *   ThirdSocks5Info;
		} Detail;
	};

	using xResultProc = void(const xResult &);

	virtual void PostRequest(const xRequest & Request);
	virtual void PeekResult(xResultProc Callback) = 0;
};
