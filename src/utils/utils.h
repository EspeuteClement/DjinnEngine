#ifndef _DJINN_UTILS_H_
#define _DJINN_UTILS_H_
#include <cstdint>

namespace djn
{
    void sleep(uint32_t time_as_ms);
    uint64_t get_time_micro();
    char * pretty_number(uint32_t number, char * buffer);
};

// Mesure time between the macro position and the end of the scope
#define DJN_PERF(str) _djn_perf_counter ___djn_counter(str)

struct _djn_perf_counter
{
    _djn_perf_counter(const char * string);
    ~_djn_perf_counter();

    uint64_t start_time;
    const char * string;
};

#endif