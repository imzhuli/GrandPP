#include "../common_protocol/network.hpp"
#include "../component_control/management_extension.hpp"

#include <iostream>
#include <pp_common/base.hpp>
using namespace std;

auto IC  = xIoContext();
auto ICG = xResourceGuard(IC);

auto BindAddress = xNetAddress::Parse("127.0.0.1", 12000);
auto ME          = xManagementExtension{};

int main(int argc, char ** argv) {

	auto MEG = xResourceGuard(ME, &IC, BindAddress);
	RuntimeAssert(MEG);

	while (true) {
		IC.LoopOnce();
		ME.Tick();
	}

	return 0;
}
