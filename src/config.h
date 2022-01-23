#ifndef CMINE_CONFIG_H_
#define CMINE_CONFIG_H_

#include <stdint.h>

// Boolean value
#define TRUE 1
#define FALSE 0

// Fixed-size types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;
typedef uint32_t b8;

// Calculates the minimum of a and b.
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

// Calculates the maximum of a and b.
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

// Calculates the absolute value of a.
#define ABS(a) (((a) < 0) ? -(a) : (a))

// Ensures that x is between the limits set by low and high.
#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

#endif // CMINE_CONFIG_H_ 
