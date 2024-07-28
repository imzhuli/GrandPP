#pragma once

#include <atomic>
#include <core/core_min.hpp>
#include <core/memory_pool.hpp>
#include <core/timer_wheel.hpp>
#include <network/io_context.hpp>

using xPPCallback     = xel::xTimerWheelCallback;
using xPPCallbackNode = xel::xTimerWheelNode;

class xPPThreadContext {
public:
	static constexpr const uint64_t DEFAULT_TIMERWHEEL_GAP_MS     = 10;
	static constexpr const size_t   DEFAULT_TIMERWHEEL_TOTAL      = 10 * 60'000 / DEFAULT_TIMERWHEEL_GAP_MS;  // 10 min
	static constexpr const size_t   DEFAULT_MAX_CALLBACK_DELEGATE = 8192;

public:
	X_MEMBER bool Init();
	X_MEMBER void Clean();
	X_MEMBER void Run();
	X_MEMBER void Stop();
	X_MEMBER void Interrupt();

	X_INLINE xel::xIoContext * GetIoContext() {
		return &IoContext;
	}

	X_MEMBER bool ScheduleNext(xPPCallback Callback, bool AutoRescheduleNext = false);
	X_MEMBER bool ScheduleImmediate(xPPCallback Callback);
	X_MEMBER bool Schedule(xPPCallback Callback, uint64_t TimeoutMS);

private:
	struct xScheduleNode : xel::xTimerWheelNode {
		xPPThreadContext * ThreadContext;
		bool               AutoRescheduleNext;
		xPPCallback        UserCallback;
		xel::xVariable     UserContext;

		~xScheduleNode() {
			X_DEBUG_PRINTF("%p", this);
		}
	};
	using xPPDelegatePool = xel::xFixedObjectPool<xScheduleNode>;
	X_MEMBER static void OnTimerWheelEvent(xel::xVariable Context, uint64_t TimestampMS);
	X_MEMBER static void OnCleanTimerWheelEvent(xel::xVariable Context, xel::xTimerWheelNode * Node);

private:
	xel::xIoContext  IoContext;
	xel::xTimerWheel TimerWheel;
	xel::xRunState   RunState;
	xPPDelegatePool  DelegatePool;
};
