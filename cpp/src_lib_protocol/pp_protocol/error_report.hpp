#pragma once
#include "./base.hpp"

struct xErrorReport : xBinaryMessage {

	void SerializeMembers() override {
		W(X2R((uint32_t)ErrorType), ErrorMessage);
	};

	void DeserializeMembers() override {
		uint32_t Type = 0;
		R(Type, ErrorMessage);
		ErrorType = (eErrorType)Type;
	};

	eErrorType  ErrorType;
	std::string ErrorMessage;
};

struct xErrorReportResp : xBinaryMessage {

	void SerializeMembers() override {
		W(Accepted);
	};

	void DeserializeMembers() override {
		R(Accepted);
	};

	bool Accepted = false;
};
