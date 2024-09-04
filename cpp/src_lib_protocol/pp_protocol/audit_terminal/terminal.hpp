#pragma once
#include <pp_common/base.hpp>

struct xTerminalAudit : xBinaryMessage {

	virtual void SerializeMembers() override {
		W(TerminalId, TerminalVersion, TerminalAddress, StartTimestampMS, OfflineTimestampMS, RelayServerId, TotalConnections, TotalUploadSize,
		  TotalDownloadSize);
	}

	virtual void DeserializeMembers() override {
		R(TerminalId, TerminalVersion, TerminalAddress, StartTimestampMS, OfflineTimestampMS, RelayServerId, TotalConnections, TotalUploadSize,
		  TotalDownloadSize);
	}

	std::string TerminalId;  // terminal device id,  length <= 96
	xNetAddress TerminalAddress;
	uint64_t    TerminalVersion;
	uint64_t    StartTimestampMS;    //
	uint64_t    OfflineTimestampMS;  // only when terminal offline is detected
	uint64_t    RelayServerId;
	uint64_t    TotalConnections;
	uint64_t    TotalUploadSize;
	uint64_t    TotalDownloadSize;
};

public
S terminalId = new S();
public
I8 version = new I8();
public
I8 startTimestampMS = new I8();  //
public
I8 offlineTimestampMS = new I8();  // only when terminal offline is detected
public
I8 relayServerId = new I8();
public
I8 totalConnections = new I8();
public
I8 totalUploadSize = new I8();
public
I8 totalDownloadSize = new I8();
public
boolean hasRegionInfo = false;
public
RegionInfo regionInfo = null;
public
InetSocketAddress addressReference = null;