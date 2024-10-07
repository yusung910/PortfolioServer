#pragma once
// 포인터를 사용하지 않는 싱글톤

template <class T>
class RefSingleton
{
public:
	static T& GetInst()
	{
		return mInst;
	}

	virtual void Init() {};

protected:
	static T mInst;
};

template <class T>
T RefSingleton<T>::mInst;