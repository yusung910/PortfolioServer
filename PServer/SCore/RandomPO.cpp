#include "stdafx.hxx"
#include "RandomPO.hxx"
#include "Clock.h"

RandomPO::RandomPO()
{
	int64_t lSeed = Clock::GetTick64();
	unsigned int lSeed32 = (unsigned int)(lSeed % 0xFFFFFFFF);

	InitMT19937_32(lSeed32, &m_nMti32, m_nMt32);
	InitMT19937_64((unsigned long long)lSeed, &m_nMti, m_lMt);
}

void RandomPO::Init64(const uint64_t& _seed)
{
	InitMT19937_64((unsigned long long)_seed, &m_nMti, m_lMt);
}

void RandomPO::Init32(const uint32_t& _seed)
{
	InitMT19937_32(_seed, &m_nMti32, m_nMt32);
}

int64_t RandomPO::GetRandom()
{
	return GenerateRandom64(&m_nMti, m_lMt);
}

int RandomPO::GetRandom32()
{
	return GenerateRandom32(&m_nMti32, m_nMt32);
}

int64_t RandomPO::GetRandomRange(const int64_t& _min, const int64_t& _max)
{
	if (_min < 0 || _max < 0)
		return 0;

	if (_min >= _max)
		return _min;

	return _min + (GenerateRandom64(&m_nMti, m_lMt) % abs(_max - _min + 1));
}

int RandomPO::GetRandomRange(const int& _min, const int& _max)
{
	if (_min < 0 || _max < 0)
		return 0;

	if (_min >= _max)
		return _min;

	return _min + (GenerateRandom32(&m_nMti32, m_nMt32) % abs(_max - _min + 1));
}

float RandomPO::GetRandomRangef(const float& _min, const float& _max)
{
	const int64_t lDivideRange = 10000000ll;	// float의 유효자릿수는 7자리
	const float lMultiplierRange = 1.0f / 10000000.0f;
	return _min + ((GenerateRandom64(&m_nMti, m_lMt) & lDivideRange) * lMultiplierRange * (_max - _min));
}

float RandomPO::GetRandomRangef(const int& _range)
{
	int32_t lDist = _range * 2 * 100;
	int32_t lSelection = GenerateRandom32(&m_nMti32, m_nMt32) % lDist;
	return ((_range * -100) + lSelection) / 100.f;
}

bool RandomPO::GetProbability10000(const int& _prob)
{
	return _GetProbability(_prob, 10000);
}

bool RandomPO::GetProbability1M(const int& _prob)
{
	return _GetProbability(_prob, 1000000);
}

bool RandomPO::_GetProbability(const int& _prob, const int& _max)
{
	if (_prob <= 0ll)
		return false;

	if (_max <= _prob)
		return true;

	return _prob >= GetRandomRange(0, _max);
}
