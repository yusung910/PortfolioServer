#pragma once
#include <cassert>

template<typename T>
class StrictSingleton
{
public:
	static T* CreateInst()
	{
		assert(nullptr == m_oInst && "Already Created Instance");
		m_oInst = new T();
		return m_oInst;
	}

	static T* GetInst()
	{
		assert(nullptr != m_oInst && "Has Not Created Instance");
		return m_oInst;
	}

	static void DestroyInst()
	{
		if (nullptr != m_oInst)
		{
			delete m_oInst;
			m_oInst = nullptr;
		}
	}

	static void SetInst(T* _inst)
	{
		m_oInst = _inst;
	}

protected:
	static T* m_oInst;
};

template<typename T>
T* StrictSingleton<T>::m_oInst = nullptr;

