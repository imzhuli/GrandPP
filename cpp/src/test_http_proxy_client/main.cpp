#include "../common/base.hpp"
#include "../common_protocol/network.hpp"
#include "../component_control/management_extension.hpp"

#include <iostream>
using namespace std;

auto IC  = xIoContext();
auto ICG = xResourceGuard(IC);

auto BindAddress = xNetAddress::Parse("127.0.0.1", 10000);
auto ME          = xManagementExtension{};

auto RequestAddress = xNetAddress::Parse("127.0.0.1", 12000);

int main(int argc, char ** argv) {

	auto MEG = xResourceGuard(ME, &IC, BindAddress);
	RuntimeAssert(MEG);

	cout << hex;

	auto RP = ME.CreateRequest();
	cout << "Start: " << GetUnixTimestamp() << endl;
	cout << "RP new: " << RP->LocalRequestId << endl;

	ME.RequestTestDelay(RequestAddress, BindAddress);

	auto T = xTimer();
	while (true) {
		IC.LoopOnce();
		ME.Tick();

		if (T.TestAndTag(3s)) {
			break;
		}
	}

	cout << "RP recycled: " << RP->LocalRequestId << endl;
	cout << "Finish: " << GetUnixTimestamp() << endl;

	return 0;
}
