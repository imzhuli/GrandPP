#pragma once
#include "./thread_context.hpp"

#include <core/core_min.hpp>

X_EXTERN bool PPInit();
X_EXTERN void PPClean();
X_EXTERN bool PPMainLoop();

X_EXTERN xPPThreadContext * PPGetMainThreadContext();
