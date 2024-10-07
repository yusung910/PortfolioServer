#pragma once
#include <cassert>

template<typename T>
class StrictSingleton
{
public:
	static T* CreateInst()
	{
		assert(nullptr == mInst && "Already Created Instance");
		mInst = new T();
		return mInst;
	}

	static T* GetInst()
	{
		assert(nullptr != mInst && "Has Not Created Instance");
		return mInst;
	}

	static void DestroyInst()
	{
		if (nullptr != mInst)
		{
			delete mInst;
			mInst = nullptr;
		}
	}

	static void SetInst(T* _inst)
	{
		mInst = _inst;
	}

protected:
	static T* mInst;
};

template<typename T>
T* StrictSingleton<T>::mInst = nullptr;

