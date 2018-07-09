#include "utils.h"
#include "windows.h"

namespace djn
{
    void sleep(uint32_t time_as_ms)
    {
        timeBeginPeriod(1);
        Sleep(time_as_ms);
    }
};
