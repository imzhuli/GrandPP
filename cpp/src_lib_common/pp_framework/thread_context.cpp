#include "./thread_context.hpp"

bool xPPThreadContext::Init() {
	if (!IoContext.Init()) {
		return false;
	}
	auto ICG = MakeResourceCleaner(IoContext);

	if (!DelegatePool.Init(DEFAULT_MAX_CALLBACK_DELEGATE)) {
		return false;
	}
	auto CDPG = MakeResourceCleaner(DelegatePool);

	if (!TimerWheel.Init(DEFAULT_TIMERWHEEL_TOTAL, DEFAULT_TIMERWHEEL_GAP_MS)) {
		return false;
	}
	auto TWG = MakeResourceCleaner(TimerWheel);

	ICG.Dismiss();
	CDPG.Dismiss();
	TWG.Dismiss();
	return true;
}

void xPPThreadContext::Clean() {
	auto ICG  = MakeResourceCleaner(IoContext);
	auto CDPG = MakeResourceCleaner(DelegatePool);
	auto TWG  = MakeResourceCleaner(TimerWheel);  // DelegateObject is simlpe and is safe not to call DelegatePool.Destroy()
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

void xPPThreadContext::OnTimerWheelEvent(xel::xVariable Context, uint64_t TimestampMS) {
	auto P = (xScheduleNode *)Context.P;
	P->UserCallback.Execute(TimestampMS);
	if (P->AutoRescheduleNext) {
		P->ThreadContext->TimerWheel.ScheduleNext(*P);
	} else {
		P->ThreadContext->DelegatePool.Destroy(P);
	}
}

void xPPThreadContext::OnCleanTimerWheelEvent(xel::xVariable Context, xel::xTimerWheelNode * Node) {
	auto P = (xPPThreadContext *)(Context.P);
	P->DelegatePool.Destroy(static_cast<xScheduleNode *>(Node));
}

bool xPPThreadContext::ScheduleNext(xPPCallback Callback, bool AutoRescheduleNext) {
	auto N = DelegatePool.Create();
	if (!N) {
		return false;
	}
	N->ThreadContext      = this;
	N->AutoRescheduleNext = AutoRescheduleNext;
	N->UserCallback       = Callback;
	SetCallback(*N, { OnTimerWheelEvent, { .P = N } });
	TimerWheel.ScheduleNext(*N);
	return true;
}

bool xPPThreadContext::ScheduleImmediate(xPPCallback Callback) {
	auto N = DelegatePool.CreateValue();
	if (!N) {
		return false;
	}
	N->ThreadContext = this;
	N->UserCallback  = Callback;
	SetCallback(*N, { OnTimerWheelEvent, { .P = N } });
	TimerWheel.ScheduleByOffset(*N, 0);
	return true;
}

bool xPPThreadContext::Schedule(xPPCallback Callback, uint64_t TimeoutMS) {
	auto N = DelegatePool.CreateValue();
	if (!N) {
		return false;
	}
	N->ThreadContext = this;
	N->UserCallback  = Callback;
	SetCallback(*N, { OnTimerWheelEvent, { .P = N } });
	TimerWheel.ScheduleByTimeoutMS(*N, TimeoutMS);
	return true;
}
