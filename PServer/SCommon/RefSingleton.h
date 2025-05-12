#pragma once
// �����͸� ������� �ʴ� �̱���
template <class T>
class RefSingleton
{
public:
	static T& GetInst()
	{
		return m_oInst;
	}

	virtual void Initialize() {};

protected:
	static T m_oInst;
};

template <class T>
T RefSingleton<T>::m_oInst;