#pragma once
// �����͸� ������� �ʴ� �̱���

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