#pragma once
#include "ObjectPool.h"
#include <memory>

template <typename T>
class Object : public std::enable_shared_from_this<T>
{
protected:
	friend ObjectPool<T>;
	struct ObjectTag {};

public:
	using UniquePtr = typename ObjectPool<T>::AutoReleaseUniuqePtr;
	using SharedPtr = typename std::shared_ptr<T>;

	virtual ~Object() = default;
	virtual void Reset() = 0;

	static UniquePtr New()
	{
#ifdef USE_MEMORY_POOL
		auto localPool = ObjectPool<T>::GetInst();
		return localPool->Acquire();
#else
		UniquePtr localPtr
		(
			new T(ObjectTag())
			, [](T* _obj) {_obj->Reset(); delete _obj; }
		);
		return localPtr;
#endif // USE_MEMORY_POOL
	}

	static void ReservePool([[maybe_unused]] const size_t& _size)
	{
#ifdef USE_MEMORY_POOL
		ObjectPool<T>::GetInst()->Reserve(_size);
#endif // USE_MEMORY_POOL

	}
};
