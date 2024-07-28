#pragma once
#include "./thread_context.hpp"

#include <core/core_min.hpp>

X_EXTERN bool PPInit();
X_EXTERN void PPClean();
X_EXTERN void PPMainLoop();
X_EXTERN void PPStopMainLoop();

X_EXTERN xPPThreadContext * PPGetMainThreadContext();
