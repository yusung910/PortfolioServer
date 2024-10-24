#pragma once
#include <stack>
#include <mutex>
#include <cstdint>
#include <functional>

template<typename T>
class ObjectPool
{
private:
	std::stack<std::unique_ptr<T>> m_oPool;
	std::recursive_mutex m_xLock;

	size_t m_nAllocationCnt = 0;
public:
	//타입 정의
	//참고 : https://cjwoov.tistory.com/68
	using AutoReleaseUniuqePtr = std::unique_ptr<T, std::function<void(T*)>>;

	static ObjectPool<T>* GetInst()
	{
		static ObjectPool<T> inst;
		return &inst;
	}


	void Reserve(size_t _size)
	{
		for (size_t i = 0; i < _size; i++)
			m_oPool.push(std::make_unique<T>(typename T::ObjectTag()));

		m_nAllocationCnt += _size;
	}

	size_t GetAllocationCount()
	{
		std::lock_guard<std::recursive_mutex> localGuard(m_xLock);

		return m_nAllocationCnt;
	}

	size_t GetUsingCnt()
	{
		std::lock_guard<std::recursive_mutex> localGuard(m_xLock);
		return m_nAllocationCnt - m_oPool.size();
	}

	AutoReleaseUniuqePtr Acquire()
	{
		std::lock_guard<std::recursive_mutex> localGuard(m_xLock);

		if (m_oPool.empty())
		{
			m_oPool.push(std::make_unique(T > (typename T::ObjectTag()));
			m_nAllocationCnt++;
		}

		AutoReleaseUniuqePtr localPtr
		(
			m_oPool.top().release()
			, [this](T* _obj) { this->Release(std::unique_ptr<T>(_obj); }
		);

		m_oPool.pop();

		return ptr;
	}

private:
	ObjectPool() = default;
	void Release(std::unique_ptr<T> _obj)
	{
		std::lock_guard<std::recursive_mutex> localGuard(m_xLock);
		_obj->Reset();
		m_oPool.push(std::move(_obj));
	}
};