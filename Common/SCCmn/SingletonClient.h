#pragma once

template<typename T>
class SingletonClient
{
public:
    //	Attributes

    //	Operations
    static T* CreateInst()
    {
#ifdef SINGLETON_STRICT
        assert(nullptr == m_oInst && "already_created_instance");
#else
        if (nullptr != m_oInst)
        {
            return m_oInst;
        }
#endif
        m_oInst = new T();
        return m_oInst;
    }

    static T* GetInst()
    {
#ifdef SINGLETON_STRICT
        assert(nullptr != m_oInst && "not_created_instance");
#else
        if (nullptr == m_oInst)
        {
            return CreateInst();
        }
#endif
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

    static void SetInstance(T* _inst)
    {
        m_oInst = _inst;
    }


protected:
    //	Attributes
    static T* m_oInst;


    //	Operations
};

template<typename T>
T* SingletonClient<T>::m_oInst = nullptr;