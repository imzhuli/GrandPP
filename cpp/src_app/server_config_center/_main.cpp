#include "./backend_server.hpp"
#include "./config.hpp"
#include "./config_service.hpp"

#include <core/core_time.hpp>
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
	GlobalConfig.Load(ConfigFile->c_str());

	RuntimeAssert(RIC);

	// TODO:
	auto CSG = xResourceGuard(CS, &IC, GlobalConfig.BindAddress);
	RuntimeAssert(CSG);

	auto BackendServerManager = xBackendServerManager();
	auto BSG                  = xResourceGuard(BackendServerManager);
	RuntimeAssert(BSG);

	auto Timer = xTimer();
	while (true) {
		IC.LoopOnce();
		CS.Tick();
		if (Timer.TestAndTag(std::chrono::seconds(60))) {
			if (!GlobalConfig.BackendServerListFilename.empty()) {
				BackendServerManager.ReloadFromFile(GlobalConfig.BackendServerListFilename.c_str());
			}
		}
	}

	return 0;
}
