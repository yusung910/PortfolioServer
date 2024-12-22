#include "mt19937.hxx"

void InitMT19937_32(unsigned int s, int* mti, unsigned int* mt)
{
	mt[0] = s & 0xffffffffU;
	for ((*mti) = 1; (*mti) < BUFFER_COUNT_32; (*mti)++)
	{
		mt[(*mti)] = (1812433253U * (mt[(*mti) - 1] ^ (mt[(*mti) - 1] >> 30)) + (*mti));
		mt[(*mti)] &= 0xffffffffU;
	}
}

int GenerateRandom32(int* mti, unsigned int* mt)
{
	unsigned int nRet;
	int kk;
	static unsigned int MAG_32[2] = { 0x0U, MATRIX_ARRAY_32 };

	if ((*mti) >= BUFFER_COUNT_32)
	{
		if ((*mti) == BUFFER_COUNT_32 + 1)
			InitMT19937_32(5489U, mti, mt);

		for (kk = 0; kk < BUFFER_COUNT_32 - M_32; kk++)
		{
			nRet = (mt[kk] & UPPER_MASK_32) | (mt[kk + 1] & LOWER_MASK_32);
			mt[kk] = mt[kk + M_32] ^ (nRet >> 1) ^ MAG_32[nRet & 0x1U];
		}

		for (; kk < BUFFER_COUNT_32 - 1; kk++)
		{
			nRet = (mt[kk] & UPPER_MASK_32) | (mt[kk + 1] & LOWER_MASK_32);
			mt[kk] = mt[kk + (M_32 - BUFFER_COUNT_32)] ^ (nRet >> 1) ^ MAG_32[nRet & 0x1U];
		}

		nRet = (mt[BUFFER_COUNT_32 - 1] & UPPER_MASK_32) | (mt[0] & LOWER_MASK_32);
		mt[BUFFER_COUNT_32 - 1] = mt[M_32 - 1] ^ (nRet >> 1) ^ MAG_32[nRet & 0x1U];

		(*mti) = 0;
	}

	nRet = mt[(*mti)++];

	nRet ^= (nRet >> 11);
	nRet ^= (nRet << 7) & 0x9d2c5680U;
	nRet ^= (nRet << 15) & 0xefc60000U;
	nRet ^= (nRet >> 18);

	return (int)(nRet & ~0x80000000);
}


void InitMT19937_64(unsigned long long seed, int* mti, unsigned long long* mt)
{
	mt[0] = seed;
	for (*mti = 1; *mti < BUFFER_COUNT_64; (*mti)++)
		mt[*mti] = (6364136223846793005ULL * (mt[*mti - 1] ^ (mt[*mti - 1] >> 62)) + *mti);
}

long long GenerateRandom64(int* mti, unsigned long long* mt)
{
	int i;
	unsigned long long nRet;
	static unsigned long long MAG_64[2] = { 0ULL, MATRIX_ARRAY_64 };

	if (*mti >= BUFFER_COUNT_64)
	{
		if (*mti == BUFFER_COUNT_64 + 1)
			InitMT19937_64(5489ULL, mti, mt);

		for (i = 0; i < BUFFER_COUNT_64 - M_64; i++)
		{
			nRet = (mt[i] & UPPER_MASK_64) | (mt[i + 1] & LOWER_MASK_64);
			mt[i] = mt[i + M_64] ^ (nRet >> 1) ^ MAG_64[(int)(nRet & 1ULL)];
		}

		for (; i < BUFFER_COUNT_64 - 1; i++)
		{
			nRet = (mt[i] & UPPER_MASK_64) | (mt[i + 1] & LOWER_MASK_64);
			mt[i] = mt[i + (M_64 - BUFFER_COUNT_64)] ^ (nRet >> 1) ^ MAG_64[(int)(nRet & 1ULL)];
		}

		nRet = (mt[BUFFER_COUNT_64 - 1] & UPPER_MASK_64) | (mt[0] & LOWER_MASK_64);
		mt[BUFFER_COUNT_64 - 1] = mt[M_64 - 1] ^ (nRet >> 1) ^ MAG_64[(int)(nRet & 1ULL)];

		*mti = 0;
	}

	nRet = mt[(*mti)++];

	nRet ^= (nRet >> 29) & 0x5555555555555555ULL;
	nRet ^= (nRet << 17) & 0x71D67FFFEDA60000ULL;
	nRet ^= (nRet << 37) & 0xFFF7EEE000000000ULL;
	nRet ^= (nRet >> 43);

	return (long long)(nRet & ~0x8000000000000000);
}
