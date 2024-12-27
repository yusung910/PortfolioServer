#include "stdafx.hxx"
#include "Random.h"
#include "RandomPO.hxx"
#include "CmnMath.h"

Random Random::m_oInst;

Random::~Random()
{
    SafeDelete(m_pRandom);
}

Random* Random::GetInst()
{
    return &m_oInst;
}

void Random::Init64(const uint64_t& _seed)
{
    //https://karupro.tistory.com/67#google_vignette
    //[[likely]] : ���ǹ��� ����ȭ �ϴ� �� ���Ǵ� ����
    //�����Ϸ����� ���ǹ��� ���� ���� ���Ǵ� �κ��� �˷��ش�
    if (nullptr != m_pRandom) [[likely]]
        m_pRandom->Init64(_seed);
}


void Random::Init32(const uint32_t& _seed)
{
    if (nullptr != m_pRandom) [[likely]]
        m_pRandom->Init32(_seed);
}

int64_t Random::GetRandom()
{
    if (nullptr == m_pRandom) [[unlikely]]
        return 0ll;

    return m_pRandom->GetRandom();
}

int Random::GetRandom32()
{
    if (nullptr == m_pRandom) [[unlikely]]
        return 0;

    return m_pRandom->GetRandom32();
}

int64_t Random::GetRandomRange(const int64_t& _min, const int64_t& _max)
{
    if (nullptr == m_pRandom) [[unlikely]]
        return 0;

    return m_pRandom->GetRandomRange(_min, _max);
}

int Random::GetRandomRange(const int& _min, const int& _max)
{
    if (nullptr == m_pRandom) [[unlikely]]
        return 0;

    return m_pRandom->GetRandomRange(_min, _max);
}

float Random::GetRandomRange(const float& _min, const float& _max)
{
    if (nullptr == m_pRandom) [[unlikely]]
        return 0.f;

    return m_pRandom->GetRandomRangef(_min, _max);
}

float Random::GetRandomRange(const int& _range)
{
    if (nullptr == m_pRandom) [[unlikely]]
        return 0.f;
    return m_pRandom->GetRandomRangef(_range);
}

bool Random::GetProbability10K(const int& _prob)
{
    if (nullptr == m_pRandom) [[unlikely]]
        return false;
    return m_pRandom->GetProbability10000(_prob);
}

bool Random::GetProbability1M(const int& _prob)
{
    if (nullptr == m_pRandom) [[unlikely]]
        return false;
    return m_pRandom->GetProbability1M(_prob);
}

float Random::RandomDegree() noexcept
{
    return m_pRandom->GetRandomRangef(0.0f, DEGREE_360);
}

float Random::RandomRadian() noexcept
{
    return m_pRandom->GetRandomRangef(0.0f, RADIAN_360);
}

Random::Random()
{
    m_pRandom = new RandomPO();
}