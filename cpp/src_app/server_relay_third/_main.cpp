#include <pp_common/base.hpp>
#include <pp_protocol/backend/auth_by_user_pass.hpp>
#include <pp_protocol/backend/backend_challenge.hpp>
#include <pp_protocol/protocol.hpp>
#include <server_arch/client.hpp>

static auto IC            = xIoContext();
static auto ICG           = xResourceGuard(IC);
static auto TestAddress   = xNetAddress::Parse("45.202.204.29:20005");
static auto TestAuth      = "B_1_US_1583_19158_30_tWkmYP8g:1234567";
static auto TestAppKey    = "apitest";
static auto TestAppSecret = "123456";

class xAuthConnection : public xClient {
public:
	bool Init(xIoContext * IoContextPtr, const xNetAddress & TargetAddress) {
		if (!xClient::Init(IoContextPtr, TargetAddress, TargetAddress.Decay())) {
			return false;
		}
		Enabled = false;
		return true;
	}

	void OnServerConnected() override {
		auto challenge           = xBackendChallenge();
		challenge.AppKey         = TestAppKey;
		challenge.TimestampMS    = xel::GetTimestampMS();
		challenge.ChallengeValue = challenge.GenerateChallengeString(TestAppSecret);
		ubyte Buffer[xel::MaxPacketSize];
		auto  RSize = xel::WritePacket(Cmd_BackendChallenge, 0, Buffer, sizeof(Buffer), challenge);
		PostData(Buffer, RSize);

		X_DEBUG_PRINTF("Sending:\n%s", HexShow(Buffer, RSize).c_str());
		X_DEBUG_PRINTF("Header: %s", StrToHex(Buffer, PacketHeaderSize).c_str());
		X_DEBUG_PRINTF("Body: %s", StrToHex(Buffer + PacketHeaderSize, RSize - PacketHeaderSize).c_str());
	}
	bool OnPacket(const xPacketHeader & Header, ubyte * PayloadPtr, size_t PayloadSize) override {
		switch (Header.CommandId) {
			case Cmd_BackendChallengeResp: {
				auto cr = xBackendChallengeResp();
				if (!cr.Deserialize(PayloadPtr, PayloadSize) || cr.ErrorCode) {
					X_DEBUG_PRINTF("Failed to challenge backend server");
					Enabled = false;
					break;
				}
				X_DEBUG_PRINTF("backend server accepted");
				Enabled = true;

				// test login:
				auto Auth     = xBackendAuthByUserPass();
				Auth.UserPass = TestAuth;
				ubyte Buffer[MaxPacketSize];
				auto  RSize = WritePacket(Cmd_AuthByUserPass, 0, Buffer, sizeof(Buffer), Auth);
				PostData(Buffer, RSize);
				break;
			}
			case Cmd_AuthByUserPassResp: {
				X_DEBUG_PRINTF("data:\n%s", HexShow(PayloadPtr, PayloadSize).c_str());
				auto crr = xBackendAuthByUserPassResp();
				if (!crr.Deserialize(PayloadPtr, PayloadSize)) {
					X_DEBUG_PRINTF("failed to parse response");
					X_DEBUG_PRINTF("AuditId: %u", (unsigned)crr.AuditId);
					X_DEBUG_PRINTF("CountryCode: %s", crr.CountryCode.c_str());
					X_DEBUG_PRINTF("StateId: %s", crr.StateId.c_str());
					X_DEBUG_PRINTF("CityId: %u", (unsigned)crr.CityId);
					X_DEBUG_PRINTF("Duration: %u", (unsigned)crr.Duration);
					X_DEBUG_PRINTF("Random: %u", (unsigned)crr.Random);
					X_DEBUG_PRINTF("Redirect: %s", crr.Redirect.c_str());
				} else {
					X_DEBUG_PRINTF("AuditId: %u", (unsigned)crr.AuditId);
					X_DEBUG_PRINTF("CountryCode: %s", crr.CountryCode.c_str());
					X_DEBUG_PRINTF("StateId: %s", crr.StateId.c_str());
					X_DEBUG_PRINTF("CityId: %u", (unsigned)crr.CityId);
					X_DEBUG_PRINTF("Duration: %u", (unsigned)crr.Duration);
					X_DEBUG_PRINTF("Random: %u", (unsigned)crr.Random);
					X_DEBUG_PRINTF("Redirect: %s", crr.Redirect.c_str());
				}
				break;
			}
			default: {
				break;
			}
		}
		return true;
	}

private:
	bool Enabled = false;
};

int main(int, char **) {
	RuntimeAssert(ICG);
	auto AC  = xAuthConnection();
	auto ACG = xResourceGuard(AC, &IC, TestAddress);
	RuntimeAssert(ACG);

	while (true) {
		IC.LoopOnce();
		AC.Tick(GetTimestampMS());
	}
	return 0;
}
