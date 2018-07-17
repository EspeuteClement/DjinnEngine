#include "utils.h"
#include "windows.h"

namespace djn
{
    void sleep(uint32_t time_as_ms)
    {
        timeBeginPeriod(1);
        Sleep(time_as_ms);
    }

    uint64_t get_time_micro()
    {
        const uint64_t TICKS_PER_SECOND = 10000000;

        static LARGE_INTEGER freq;
        if (freq.QuadPart == 0)
            QueryPerformanceFrequency(&freq);
        
        // FILETIME ft;
        // GetSystemTimeAsFileTime(&ft);

        // uint64_t us = ft.dwHighDateTime; 
        // us <<= 32;
        // us |= ft.dwLowDateTime;

        // us /= 10;

        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);



        return li.QuadPart/(freq.QuadPart/1000000.0);
    }
};
