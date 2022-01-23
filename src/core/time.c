#include "time.h"

#if defined(__linux__) || defined (__APPLE__)
#define _POSIX_C_SOURCE 199309L
#include <sys/time.h>
#endif

#include <time.h>

#if defined(_WIN32)
#include <windows.h>

static f64 clock_frequency;
static LARGE_INTEGER start_time;

void time_init() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clock_frequency = 1.0 / (f64)frequency.QuadPart;
    QueryPerformanceCounter(&start_time);
}
#endif

f64 time_now() {
#if defined(__linux__) || defined (__APPLE__)
	struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec * 0.000000001;
#endif

#if defined(_WIN32)
	if (!clock_frequency) {
      time_init();
    }

    LARGE_INTEGER now_time;
    QueryPerformanceCounter(&now_time);
    return (f64)now_time.QuadPart * clock_frequency;
#endif
}

