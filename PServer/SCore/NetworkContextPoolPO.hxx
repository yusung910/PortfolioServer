#pragma once
#include <mutex>
#include <deque>

class NetworkContextPO;

class NetworkContextPoolPO
{
private:
    std::mutex m_xLock;
    std::deque<NetworkContextPO*> m_oAllocatedList;
    std::deque<NetworkContextPO*> m_oFreeList;

    size_t m_nAllocatedSize = 0;

public:
    /*!
     *  Constructor.
     *  Reserve()함수로 인자값 크기만큼 pool을 할당한다
     *      @param [in] _reserveSize 
     */
    NetworkContextPoolPO(size_t _reserveSize = 100);

    /*!
     *  m_oAllocatedList, m_oFreeList를 초기화 시킨다
     *  Destructor.
     */
    virtual ~NetworkContextPoolPO();

public:
    /*!
     *  NetworkContextPO를 할당하고 반환한다
     *
     *      @return 
     */
    NetworkContextPO* Allocate();

    /*!
     *  인자값으로 전달받은 NetworkContextPO를
     *  m_oFreeList에 넣는다
     *  사용 대기 상태로 지정
     *      @param [in,out] _ctxt 
     */
    void Release(NetworkContextPO* _ctxt);

    /*!
     *  인자값으로 전달받은 _size 수 만큼
     *  NetworkContextPO를 저장할 m_oAllocatedList, m_oFreeList에 생성한다
     *
     *      @param [in] _size 
     */
    void Reserve(size_t _size = 100);

    /*!
     *  Returns the network context pool's allocated count.
     *
     *      @return The allocated count.
     */
    const size_t GetAllocatedCount() const;

    /*!
     *  Returns the network context pool's usage.
     *
     *      @param [in,out] _free      
     *      @param [in,out] _allocated 
     */
    void GetUsage(size_t& _free, size_t& _allocated);
};

