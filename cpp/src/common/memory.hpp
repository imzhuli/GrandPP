#pragma once
#include "./base.hpp"

template <typename T>
class xPoolAllocator : public xAllocator {
private:
	struct xHolder {
		alignas(T) ubyte _[sizeof(T)];
	};
	xMemoryPool<xHolder> Pool;
	static_assert(std::is_trivial_v<xHolder>);
	static_assert(sizeof(xHolder) == sizeof(T));

public:
	xPoolAllocator(const xMemoryPoolOptions & Opt) {
		RuntimeAssert(Pool.Init(Opt));
	}
	~xPoolAllocator() {
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

template <typename T>
class xStaticPoolAllocator final : public xPoolAllocator<T> {
private:
	using xBase = xPoolAllocator<T>;
	struct xHolder {
		alignas(T) ubyte _[sizeof(T)];
	};
	static_assert(std::is_trivial_v<xHolder>);
	static_assert(sizeof(xHolder) == sizeof(T));

public:
	xStaticPoolAllocator(size_t PoolSize)
		: xBase(xMemoryPoolOptions{
			  .InitSize          = PoolSize,
			  .Addend            = 0,
			  .MultiplierBy100th = 0,
			  .MaxSizeIncrement  = 0,
			  .MaxPoolSize       = PoolSize,
		  }) {
	}
	~xStaticPoolAllocator() {
	}
};
