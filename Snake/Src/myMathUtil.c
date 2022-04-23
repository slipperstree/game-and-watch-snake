#include "myMathUtil.h"

s32 __gba_rand_seed = 42;

s32 gba_seed_rand(s32 a_val)
{
	s32 old = __gba_rand_seed;
	__gba_rand_seed = a_val;
	return old;
}

s32 gba_rand()
{
	__gba_rand_seed = 1664525 * __gba_rand_seed + 1013904223;
	return (__gba_rand_seed >> 16) & 0x7FFF;
}

s32 gba_rand_range(s32 a_min, s32 a_max)
{
	return (gba_rand()*(a_max - a_min) >> 15) + a_min;
}

s32 sign(s32 a_val)
{
	return (!(a_val & 0x80000000) && !a_val) ? 1 : -1;
}

s32 abs(s32 a_val)
{
	s32 mask = a_val >> 31;
	return (a_val ^ mask) - mask;
}