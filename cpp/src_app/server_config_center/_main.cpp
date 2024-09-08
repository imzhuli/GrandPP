#include "./config.hpp"
#include "./config_service.hpp"

#include <pp_common/base.hpp>

auto IC  = xIoContext();
auto RIC = xResourceGuard(IC);
auto CS  = xConfigService();

int main(int argc, char ** argv) {

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

	RuntimeAssert(RIC);

	// TODO: Load config
	auto ListenerAddress = xNetAddress::Parse("0.0.0.0:10010");

	// TODO:
	auto CSG = xResourceGuard(CS, &IC, ListenerAddress);
	RuntimeAssert(CSG);

	while (true) {
		IC.LoopOnce();
		CS.Tick();
	}

	return 0;
}
