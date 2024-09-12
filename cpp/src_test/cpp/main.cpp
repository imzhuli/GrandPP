#include <core/core_min.hpp>
#include <core/timer_wheel.hpp>
#include <iostream>
#include <network/net_address.hpp>
#include <pp_common/hostname.hpp>
#include <pp_protocol/config_center/dns_dispatcher.hpp>
#include <pp_protocol/config_center/dns_server.hpp>
#include <pp_protocol/dns_server/dns_dispatcher.hpp>

using namespace std;
using namespace xel;

int main(int, char **) {

	ubyte Buffer[1024];

	auto R       = xCheckinDnsServer();
	R.GroupId    = 1024;
	R.VersionKey = "Hello world!";
	auto RSize   = R.Serialize(Buffer, sizeof(Buffer));
	cout << RSize << endl;

	auto RR = xCheckinDnsServer();
	auto D  = RR.Deserialize(Buffer, sizeof(Buffer));
	cout << D << endl;
	cout << RR.GroupId << endl;
	cout << RR.UnixTimestamp << endl;
	cout << RR.VersionKey << endl;

	auto RE = xGetDnsServerGroupCount();
	// auto RESize = RE.Serialize(Buffer, sizeof(Buffer));
	// cout << "RESize: " << RESize << endl;
	auto RRESize = RE.Deserialize(Buffer, sizeof(Buffer));
	cout << "RESize: " << RRESize << endl;

	return 0;
}
