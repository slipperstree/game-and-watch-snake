#ifndef __GBA_MATHUTIL_H__
#define __GBA_MATHUTIL_H__

#include "def_types.h"

extern s32 __gba_rand_seed;
extern s32 gba_seed_rand(s32 a_val);
extern s32 gba_rand();
extern s32 gba_rand_range(s32 a_min, s32 a_max);
extern s32 sign(s32 a_val);
extern s32 abs(s32 a_val);

#endif //__GBA_MATHUTIL_H__
