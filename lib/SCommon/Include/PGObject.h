#pragma once
#include "PGObjectPool.h"
#include <memory>

template <typename T>
class PGObject : public std::enable_shared_from_this<T>
{
protected:
	friend PGObjectPool<T>;
	struct ObjectTag {};

public:
	using UniquePtr = typename PGObjectPool<T>::AutoReleaseUniuqePtr;
	using SharedPtr = typename std::shared_ptr<T>;

	virtual ~PGObject() = default;
	virtual void Reset() = 0;

	static UniquePtr New()
	{
#ifdef USE_MEMORY_POOL
		auto lPool = PGObjectPool<T>::GetInst();
		return lPool->Acquire();
#else
		UniquePtr lPtr
		(
			new T(ObjectTag())
			, [](T* _obj) {_obj->Reset(); delete _obj; }
		);
		return lPtr;
#endif // USE_MEMORY_POOL
	}

	static void ReservePool([[maybe_unused]] const size_t& _size)
	{
#ifdef USE_MEMORY_POOL
        PGObjectPool<T>::GetInst()->Reserve(_size);
#endif // USE_MEMORY_POOL

	}
};
