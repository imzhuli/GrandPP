#include "./error_service.hpp"

#include <pp_common/base.hpp>

auto IC  = xIoContext();
auto RIC = xResourceGuard(IC);
auto CS  = xErrorService();

int main(int, char **) {
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
