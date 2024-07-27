#pragma once
#include "./thread_context.hpp"

#include <core/core_min.hpp>

class xPPObject : xel::xAbstract {
public:
	xPPThreadContext * GetThreadContext() const;
	virtual void       SetThreadContext(xPPThreadContext * TC);

public:
	xPPThreadContext * ThreadContext = nullptr;
};
