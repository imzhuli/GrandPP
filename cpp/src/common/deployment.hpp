#pragma once
#include "./base.hpp"

enum eDeploymentState : uint8_t {
	UNSPEC               = 0,
	CONFIG_INPROGRESS    = 1,
	CONFIG_READY         = 2,
	DEPLOYMENT_INPROGRES = 5,
	RUNNING              = 10,
	HALTED               = 100,
};
