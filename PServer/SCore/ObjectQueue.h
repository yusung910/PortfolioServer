#pragma once
#include <deque>
#include <mutex>


template<typename T>
class ObjectQueue
{
private:
	std::recursive_mutex m_xLock;
	std::deque<T> m_oObjects;

public:
	void Clear()
	{
		std::lock_guard<std::recursive_mutex> localGuard(m_xLock);
		m_oObjects.clear();
	}

	size_t Count()
	{
		std::lock_guard<std::recursive_mutex> localGuard(m_xLock);
		return m_oObjects.size();
	}

public:
	void Push(T _obj)
	{
		std::lock_guard<std::recursive_mutex> localGuard(m_xLock);
		m_oObjects.push_back(_obj);
	}

	T Pop()
	{
		T localObj = T();

		std::lock_guard<std::recursive_mutex> localGuard(m_xLock);
		if (!m_oObjects.empty())
		{
			localObj = m_oObjects.front();
			m_oObjects.pop_front();
		}

		return localObj;
	}

	void Swap(std::deque<T>& _List)
	{
		std::lock_guard<std::recursive_mutex> localGuard(m_xLock);
		m_oObjects.swap(_List);
	}

	void Get(std::deque<T>& _List, const size_t& _getSize)
	{
		_List.clear();

		std::lock_guard<std::recursive_mutex> localGuard(m_xLock);
		if (m_oObjects.size() <= _getSize)
		{
			m_oObjects.swap(_List);
		}
		else
		{
			_List.assign(m_oObjects.begin(), m_oObjects.begin() + _getSize);
			m_oObjects.erase(m_oObjects.begin(), m_oObjects.begin() + _getSize);
		}
	}
};