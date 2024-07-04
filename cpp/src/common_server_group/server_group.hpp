#pragma once
#include "../common/base.hpp"

class xServerGroup;
class xServerConnectionGroup;

struct xServerInfo {
	xServerId   ServerId;
	xGroupId    GroupId;
	xNetAddress Address;
};

class xServerGroup {
public:
	void SetGroupId(xGroupId GID) {
		GroupId = GID;
	}
	xGroupId GetGroupId() const {
		return GroupId;
	}
	void AddServer(const xServerInfo * SI) {
		assert(SI->GroupId == GroupId);
		ServerList.push_back(SI);
	}
	void RemoveServer(xServerId SID) {
		for (auto I = ServerList.begin(); I != ServerList.end(); ++I) {
			if ((*I)->ServerId == SID) {
				ServerList.erase(I);
				return;
			}
		}
	}
	const xServerInfo * GetAnyServer() {
		auto Total = ServerList.size();
		if (!Total) {
			return nullptr;
		}
		return ServerList[(LastSelected = (LastSelected + 1) % Total)];
	}
	size_t GetSize() const {
		return ServerList.size();
	}

private:
	xGroupId                         GroupId;
	std::vector<const xServerInfo *> ServerList;
	size_t                           LastSelected = 0;
};

class xServerConnection
	: public xListNode
	, public xClient {
public:
	xServerConnection(xIoContext * ICP, const xServerInfo & ServerInfo);
	~xServerConnection();

protected:
	bool OnPacket(const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) override;

private:
	friend class xServerConnectionGroup;
	xServerConnectionGroup * SCGP = nullptr;
};

class xServerConnectionGroup : xNonCopyable {
public:
	xServerConnectionGroup() = default;
	~xServerConnectionGroup();

	void Tick(uint64_t NowMS);

	void SetGroupId(xGroupId GID) {
		GroupId = GID;
	}
	xGroupId GetGroupId() const {
		return GroupId;
	}

	bool OnPacket(const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) {
		return true;
	}

private:
	xGroupId                       GroupId;
	std::vector<xServerConnection> ServerList;
	xList<xServerConnection>       ConnectionList;
};
