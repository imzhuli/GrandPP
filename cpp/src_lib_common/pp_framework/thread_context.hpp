#pragma once
#include <core/core_min.hpp>
#include <core/timer_wheel.hpp>
#include <network/io_context.hpp>

class xPPThreadContext {
public:
	xel::xIoContext  IoContext;
	xel::xTimerWheel TimerWheel;

public:
	static constexpr const uint64_t DEFAULT_TIMERWHEEL_GAP_MS = 10;
	static constexpr const size_t   DEFAULT_TIMERWHEEL_TOTAL  = 10 * 60'000 / DEFAULT_TIMERWHEEL_GAP_MS;  // 10 min

public:
	X_MEMBER bool Init();
	X_MEMBER void Clean();
	X_MEMBER void LoopOnce();
};
