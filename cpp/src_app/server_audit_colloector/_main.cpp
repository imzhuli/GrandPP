#include "./config.hpp"
#include "./report_connection.hpp"
#include "pp_common/base.hpp"

static auto C   = xAuditControllerConfig();
static auto IC  = xIoContext();
static auto ICG = xResourceGuard(IC);

int main(int argc, char ** argv) {

	auto N = xNetAddress::Parse("45.202.204.29:20005");
	cout << N.ToString() << endl;

	auto CL = xCommandLine(
		argc, argv,
		{
			{ 'c', "config", "config", true },
		}
	);
	auto ConfigFile = CL["config"];
	if (!ConfigFile()) {
		cerr << "Failed to get config file" << endl;
		QuickExit();
	}
	C.Load(ConfigFile->c_str());

	RuntimeAssert(ICG);
	auto AC  = xAuditReportConnection();
	auto ACG = xResourceGuard(AC, &IC, C.TestAddress, C.TestAppKey, C.TestAppSecret);
	RuntimeAssert(ACG);

	while (true) {
		IC.LoopOnce();
		AC.Tick();
	}

	return 0;
}
