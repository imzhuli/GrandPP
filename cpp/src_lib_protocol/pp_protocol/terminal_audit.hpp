#pragma once
#include "./base.hpp"

struct xTerminalAudit : xBinaryMessage {

	X_API_MEMBER virtual void SerializeMembers() override {
		W(TerminalId, StartTimestampMS, OfflineTimestampMS, RelayServerId, TotalConnections, TotalUploadSize, TotalDownloadSize);
	}

	X_API_MEMBER virtual void DeserializeMembers() override {
		W(TerminalId, StartTimestampMS, OfflineTimestampMS, RelayServerId, TotalConnections, TotalUploadSize, TotalDownloadSize);
	}

	std::string TerminalId;          // terminal device id,  length <= 96
	uint64_t    StartTimestampMS;    //
	uint64_t    OfflineTimestampMS;  // only when terminal offline is detected
	uint64_t    RelayServerId;
	uint64_t    TotalConnections;
	uint64_t    TotalUploadSize;
	uint64_t    TotalDownloadSize;
};
