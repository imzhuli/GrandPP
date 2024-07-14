#pragma once
#include "./base.hpp"

template <typename T>
class xStaticPoolAllocator final : public xAllocator {
private:
	struct xHolder {
		alignas(T) ubyte _[sizeof(T)];
	};
	xMemoryPool<xHolder> Pool;
	static_assert(std::is_trivial_v<xHolder>);
	static_assert(sizeof(xHolder) == sizeof(T));

public:
	xStaticPoolAllocator(size_t PoolSize) {
		auto Opt = xel::xMemoryPoolOptions{
			.InitSize          = PoolSize,
			.Addend            = 0,
			.MultiplierBy100th = 0,
			.MaxSizeIncrement  = 0,
			.MaxPoolSize       = PoolSize,
		};
		RuntimeAssert(Pool.Init(Opt));
	}
	~xStaticPoolAllocator() {
		Pool.Clean();
	}

protected:
	void * Alloc(size_t vxSize, size_t vxAlignment) noexcept override {
		assert(vxSize == sizeof(xHolder));
		if (auto P = Pool.Create()) {
			return &P->_;
		}
		return nullptr;
	}
	void Free(void * vpObject) noexcept override {
		auto P = X_Entry(vpObject, xHolder, _);
		Pool.Destroy(P);
	}
};
