#include <chrono>
#include <core/core_time.hpp>
#include <iostream>
#include <pp_framework/framework.hpp>
#include <pp_stub/dns._local.hpp>
#include <thread>

using namespace xel;
using namespace std;

xPPDnsServiceLocal SL;

void ProcessDnsResult(xVariable Context, uint64_t TimestampMS) {
	auto Iter = SL.GetResultIterator();
	while (auto R = Iter.Next()) {
		cout << R->A4.IpToString() << endl;
	}
}

uint64_t StartTimeMS = 0;
void     Terminate(xVariable, uint64_t TimeoutMS) {
    X_DEBUG_PRINTF("Timeout: %i", (int)(TimeoutMS - StartTimeMS));
    PPStopMainLoop();
}

int main(int argc, char ** argv) {
	StartTimeMS = GetTimestampMS();
	PPInit();
	auto MTC = PPGetMainThreadContext();
	MTC->Schedule(Terminate, 3000);

	SL.Init(10, MTC);
	MTC->ScheduleNext(ProcessDnsResult, true);

	SL.PostDnsRequest({}, "www.qq.com");
	SL.PostDnsRequest({}, "www.baidu.com");
	SL.PostDnsRequest({}, "www.netease.com");

	PPMainLoop();

	SL.Clean();
	PPClean();
	return 0;
}
