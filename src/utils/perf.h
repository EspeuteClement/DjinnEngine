#pragma once
#include <cstdint>

// Mesure time between the macro position and the end of the scope
#define DJN_PERF(str) _djn_perf_counter ___djn_counter(str)

struct _djn_perf_counter
{
    _djn_perf_counter(const char * string);
    ~_djn_perf_counter();

    uint64_t start_time;
    const char * string;
};