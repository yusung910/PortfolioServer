extern "C"
{
#include "mt19937.hxx"
}

class RandomPO
{
private:
	//
	unsigned int m_nMt32[BUFFER_COUNT_32];
	int m_nMti32 = BUFFER_COUNT_32 + 1;

	unsigned long long m_lMt[BUFFER_COUNT_64];
	int m_nMti = BUFFER_COUNT_64 + 1;

public:
	RandomPO();
	~RandomPO() = default;

	void Init64(const uint64_t& _seed);
	void Init32(const uint32_t& _seed);

	int64_t GetRandom();
	int GetRandom32();

	int64_t GetRandomRange(const int64_t& _min, const int64_t& _max);
	int GetRandomRange(const int& _min, const int& _max);
	float GetRandomRangef(const float& _min, const float& _max);

	float GetRandomRangef(const int& _range);

	bool GetProbability10000(const int& _prob);
	bool GetProbability1M(const int& _prob);

private:

	bool _GetProbability(const int& _prob, const int& _max);
};

