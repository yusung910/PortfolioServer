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
     *  Reserve()�Լ��� ���ڰ� ũ�⸸ŭ pool�� �Ҵ��Ѵ�
     *      @param [in] _reserveSize 
     */
    NetworkContextPoolPO(size_t _reserveSize = 100);

    /*!
     *  m_oAllocatedList, m_oFreeList�� �ʱ�ȭ ��Ų��
     *  Destructor.
     */
    virtual ~NetworkContextPoolPO();

public:
    /*!
     *  NetworkContextPO�� �Ҵ��ϰ� ��ȯ�Ѵ�
     *
     *      @return 
     */
    NetworkContextPO* Allocate();

    /*!
     *  ���ڰ����� ���޹��� NetworkContextPO��
     *  m_oFreeList�� �ִ´�
     *  ��� ��� ���·� ����
     *      @param [in,out] _ctxt 
     */
    void Release(NetworkContextPO* _ctxt);

    /*!
     *  ���ڰ����� ���޹��� _size �� ��ŭ
     *  NetworkContextPO�� ������ m_oAllocatedList, m_oFreeList�� �����Ѵ�
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

