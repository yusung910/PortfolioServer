#pragma once
#include <mutex>
#include <queue>

//template <class T>와 template <typename T>의 기능상 차이는 없음
//식별하기 위한 용도로 많이 사용되는 것으로 확인
//https://m.blog.naver.com/oh-mms/222030206308
template <class T>
class BlockingQueue
{
private:
    std::recursive_mutex m_oLock;
    std::queue<T*> m_oQueue;

public:
    BlockingQueue() = default;
    ~BlockingQueue() { clear(); }

    size_t size()
    {
        std::lock_guard<std::recursive_mutex> localGuard(m_oLock);
        return m_oQueue.size();
    }

    void clear()
    {
        T* data = nullptr;

        std::lock_guard<std::recursive_mutex> localGuard(m_oLock);

        while (false == m_oQueue.empty())
        {
            m_oQueue.pop();
            if (nullptr != data)
            {
                delete data;
                data = nullptr;
            }
        }
    }

    void Push(T* _data)
    {
        if (nullptr == _data) return;
        std::lock_guard<std::recursive_mutex> localGuard(m_oLock);
        m_oQueue.push(_data);
    }

    T* Pop()
    {
        T* data = nullptr;
        {
            std::lock_guard<std::recursive_mutex> localGuard(m_oLock);
            if (false == m_oQueue.empty())
            {
                data = m_oQueue.front();
                m_oQueue.pop();
            }
        }

        return data;
    }

    T* Peek()
    {
        T* localpRet = nullptr;
        {
            std::lock_guard<std::recursive_mutex> localGuard(m_oLock);
            if (false == m_oQueue.empty())
                localpRet = m_oQueue.front();
        }

        return localpRet;
    }

    void GetList(std::vector<T*>& _oList)
    {
        std::lock_guard<std::recursive_mutex> localGuard(m_oLock);

        while(false == m_oQueue.empty())
        {
            _oList.push_back(m_oQueue.front());
            m_oQueue.pop();
        }
    }
};