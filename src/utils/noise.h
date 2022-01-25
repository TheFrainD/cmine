#ifndef CMINE_NOISE_H_
#define CMINE_NOISE_H_

#include <math.h>

// Code was kindly copied from https://gist.github.com/nowl/828013
#include "config.h"

void noise_seed(i32 seed);
i32 noise2(i32 x, i32 y);
f32 lin_inter(f32 x, f32 y, f32 s);
f32 smooth_inter(f32 x, f32 y, f32 s);
f32 noise2d(f32 x, f32 y);
f32 perlin2d(f32 x, f32 y, f32 freq, i32 depth);

float perlin(float x, float y);

#endif // CMINE_NOISE_H_