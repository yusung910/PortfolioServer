#pragma once

template<typename T>
class Singleton
{
public:
	static T* CreateInst()
	{
		if (nullptr != m_oInst) return m_oInst;

		m_oInst = new T();
		return m_oInst;
	}

	static T* GetInst()
	{
		if (nullptr == m_oInst) return CreateInst();
		
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
T* Singleton<T>::m_oInst = nullptr;



