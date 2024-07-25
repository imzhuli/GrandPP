#include "./thread_context.hpp"

bool xPPThreadContext::Init() {
	if (!IoContext.Init()) {
		return false;
	}
	auto ICG = MakeResourceCleaner(IoContext);

	if (!TimerWheel.Init()) {
		return false;
	}
	auto TWG = MakeResourceCleaner(TimerWheel);

	ICG.Dismiss();
	TWG.Dismiss();
	return true;
}

void xPPThreadContext::Clean() {
	auto ICG = MakeResourceCleaner(IoContext);
	auto TWG = MakeResourceCleaner(TimerWheel);
}

void xPPThreadContext::LoopOnce() {
	IoContext.LoopOnce();
	TimerWheel.Forward();
}
