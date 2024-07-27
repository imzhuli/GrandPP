#include "./thread_context.hpp"

bool xPPThreadContext::Init() {
	if (!IoContext.Init()) {
		return false;
	}
	auto ICG = MakeResourceCleaner(IoContext);

	if (!TimerWheel.Init(DEFAULT_TIMERWHEEL_TOTAL, DEFAULT_TIMERWHEEL_GAP_MS)) {
		return false;
	}
	auto TWG = MakeResourceCleaner(TimerWheel);

	if (!DelegatePool.Init(DEFAULT_MAX_CALLBACK_DELEGATE)) {
		return false;
	}
	auto CDPG = MakeResourceCleaner(DelegatePool);

	ICG.Dismiss();
	TWG.Dismiss();
	CDPG.Dismiss();
	return true;
}

void xPPThreadContext::Clean() {
	auto ICG  = MakeResourceCleaner(IoContext);
	auto TWG  = MakeResourceCleaner(TimerWheel);
	auto CDPG = MakeResourceCleaner(DelegatePool);
}

void xPPThreadContext::Run() {
	xel::RuntimeAssert(RunState.Start());
	while (RunState) {
		IoContext.LoopOnce();
		TimerWheel.Forward();
	}
	RunState.Finish();
}

void xPPThreadContext::Stop() {
	RunState.Stop();
}

void xPPThreadContext::Interrupt() {
	IoContext.Interrupt();
}

bool xPPThreadContext::Schedule(xPPCallback Callback, xel::xVariable Context, uint64_t TimeoutMS) {
	// TODO:
	// 1. Create Callback Object

	// 2. do schedule

	return false;
}
