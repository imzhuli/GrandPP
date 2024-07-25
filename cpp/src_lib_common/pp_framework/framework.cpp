#include "./framework.hpp"

#include <mutex>
#include <thread>

using namespace xel;
using namespace std;

static bool  PPInited = false;
static mutex PPInitMutex;

static xPPThreadContext MainThreadContext;

bool PPInit() {
	auto G = lock_guard(PPInitMutex);
	if (!MainThreadContext.Init()) {
		return false;
	}
	auto ICG = MakeResourceCleaner(MainThreadContext);

	ICG.Dismiss();
	return (PPInited = true);
}

void PPClean() {
	auto G   = lock_guard(PPInitMutex);
	auto ICG = MakeResourceCleaner(MainThreadContext);
	PPInited = false;
}

xPPThreadContext * PPGetMainThreadContext() {
	return &MainThreadContext;
}
