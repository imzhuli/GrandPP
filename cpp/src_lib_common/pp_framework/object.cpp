#include "./object.hpp"

xPPThreadContext * xPPObject::GetThreadContext() const {
	return this->ThreadContext;
}

void xPPObject::SetThreadContext(xPPThreadContext * TC) {
	this->ThreadContext = TC;
}
