#ifndef _MT19937_H
#define _MT19937_H

// for 32 bit
#define BUFFER_COUNT_32 624
#define M_32 397
#define MATRIX_ARRAY_32 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK_32 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK_32 0x7fffffffUL /* least significant r bits */

// for  64 bit
#define BUFFER_COUNT_64 312
#define M_64 156
#define MATRIX_ARRAY_64 0xB5026F5AA96619E9ULL
#define UPPER_MASK_64 0xFFFFFFFF80000000ULL /* Most significant 33 bits */
#define LOWER_MASK_64 0x7FFFFFFFULL /* Least significant 31 bits */


extern void InitMT19937_32(unsigned int seed, int* mti, unsigned int* mt);
extern int GenerateRandom32(int* mti, unsigned int* mt);

extern void InitMT19937_64(unsigned long long seed, int* mti, unsigned long long* mt);
extern long long GenerateRandom64(int* mti, unsigned long long* mt);

#endif