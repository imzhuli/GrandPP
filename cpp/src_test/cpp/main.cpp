#include <core/core_min.hpp>
#include <core/timer_wheel.hpp>
#include <iostream>
#include <network/net_address.hpp>

using namespace std;
using namespace xel;

int main(int, char **) {

	auto A = xNetAddress();
	auto B = xNetAddress();

	cout << (A != B) << endl;

	return 0;
}
