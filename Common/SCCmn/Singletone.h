#pragma once


template<typename T>
class Singleton
{
public:
	static T* CreateInst()
	{
		if (nullptr != mInst) return mInst;

		mInst = new T();
		return mInst;
	}

	static T* GetInst()
	{
		if (nullptr == mInst) return CreateInst();
		
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
T* Singleton<T>::mInst = nullptr;



