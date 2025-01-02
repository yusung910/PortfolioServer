#pragma once
// 포인터를 사용하지 않는 싱글톤
template <class T>
class RefSingleton
{
public:
	static T& GetInst()
	{
		return m_oInst;
	}

	virtual void Init() {};

protected:
	static T m_oInst;
};

template <class T>
T RefSingleton<T>::m_oInst;