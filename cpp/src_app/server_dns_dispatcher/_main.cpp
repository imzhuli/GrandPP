#include "./config.hpp"

#include <pp_common/base.hpp>
#include <server_arch/service.hpp>

struct xDnsDispatcherServer : public xService {};

static xIoContext           IC;
static xDnsDispatcherServer Server;

int main(int argc, char ** argv) {

	auto CL = xCommandLine(
		argc, argv,
		{
			{ 'c', nullptr, "config", true },
		}
	);
	auto C = CL["config"];
	if (!C()) {
		cerr << "missing config filename" << endl;
		X_PFATAL("missing config filename: param -c config_filename");
	}

	GlobalConfig.Load(C->c_str());

	auto ICG = xResourceGuard(IC);
	auto SG  = xResourceGuard(Server, &IC, GlobalConfig.ConsumerBindAddress);
	if (!SG) {
		X_PFATAL("failed to bind address");
	}

	while (true) {
		IC.LoopOnce();
		Server.Tick();
	}

	return 0;
}
