#include <chrono>
#include <iostream>
#include <pp_stub/dns._local.hpp>
#include <thread>

using namespace xel;
using namespace std;

int main(int argc, char ** argv) {

	xPPDnsServiceLocal SL;
	SL.Init(1);

	SL.PostDnsRequest({}, "www.qq.com");
	for (int i = 0; i < 3; ++i) {
		this_thread::sleep_for(chrono::seconds(1));
		auto Iter = SL.GetResultIterator();
		while (auto R = Iter.Next()) {
			cout << R->A4.IpToString() << endl;
		}
	}

	SL.Clean();
	return 0;
}
