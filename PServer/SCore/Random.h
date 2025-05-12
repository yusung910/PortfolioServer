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
    //더 이상 사용되지 않거나 추후 수정사항이 없는 함수에 대한 경고 메세지를 출력할 때 사용
    [[deprecated("Use GetRandomRange(float, float) instead.")]]
    float GetRandomRange(const int& _range);

    bool GetProbability10K(const int& _prob);
    bool GetProbability1M(const int& _prob);

    float RandomDegree() noexcept;
    float RandomRadian() noexcept;

private:
    //https://mufflemumble.tistory.com/14
    //생성자를 별도로 private로 선언할 때
    //static inst로 생성된 객체를 리턴한다
    Random();
};

