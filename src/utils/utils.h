#ifndef _DJINN_UTILS_H_
#define _DJINN_UTILS_H_
#include <cstdint>

namespace djn
{
    void sleep(uint32_t time_as_ms);
    uint64_t get_time_micro();
    char * pretty_number(uint32_t number, char * buffer);
};

#endif