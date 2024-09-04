#include "./report_connection.hpp"
#include "pp_common/base.hpp"

static auto IC            = xIoContext();
static auto ICG           = xResourceGuard(IC);
static auto TestAddress   = xNetAddress::Parse("");
static auto TestAppKey    = "apitest";
static auto TestAppSecret = "123456";

int main(int, char **) {
	RuntimeAssert(ICG);
	auto AC  = xAuditReportConnection();
	auto ACG = xResourceGuard(AC, &IC, TestAddress, TestAppKey, TestAppSecret);
	RuntimeAssert(ACG);

	while (true) {
		IC.LoopOnce();
		AC.Tick();
	}

	return 0;
}
