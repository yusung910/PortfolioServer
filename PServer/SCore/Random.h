#pragma once
#include "SCoreAPI.h"

class RandomPO;

class SCoreAPI Random
{
private:
    static Random m_oInst;
    RandomPO* m_pRandom = nullptr;

public:
    ~Random();
    static Random* GetInst();
    void Init64(const uint64_t& _seed);
    void Init32(const uint32_t& _seed);

    int64_t GetRandom();
    int GetRandom32();

    int64_t GetRandomRange(const int64_t& _min, const int64_t& _max);
    int GetRandomRange(const int& _min, const int& _max);
    float GetRandomRange(const float& _min, const float& _max);

    //[[deprecated("Use GetRandomRange(float, float) instead.")]]
    //�� �̻� ������ �ʰų� ���� ���������� ���� �Լ��� ���� ��� �޼����� ����� �� ���
    [[deprecated("Use GetRandomRange(float, float) instead.")]]
    float GetRandomRange(const int& _range);

    bool GetProbability10K(const int& _prob);
    bool GetProbability1M(const int& _prob);

    float RandomDegree() noexcept;
    float RandomRadian() noexcept;

private:
    //https://mufflemumble.tistory.com/14
    //�����ڸ� ������ private�� ������ ��
    //static inst�� ������ ��ü�� �����Ѵ�
    Random();
};

