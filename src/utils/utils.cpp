#include "utils.h"
#ifdef _GLFW_WIN32
    #include "utils.32.cpp"
#endif

#include <cmath>
#include <cstdio>

char * pretty_number(uint32_t number, char * buffer)
{
    uint16_t size = ((uint16_t) log10(number)) + 1;
    uint16_t pos_in_buffer = 0;
    uint32_t divider = pow(10, size-1);
    uint16_t i = size;
	uint32_t new_number = number;
    while(i > 0)
    {
		char new_char = '0' + (uint16_t)(new_number / divider);
		buffer[pos_in_buffer] = new_char;
		new_number %= divider;
        
		divider /= 10;
        pos_in_buffer ++;
        
        --i;
        if (i % 3 == 0 && i != 0)
        {
			buffer[pos_in_buffer] = ' ';
            pos_in_buffer ++;
        }
    }
    buffer[pos_in_buffer] = 0;

    return buffer;
}

_djn_perf_counter::_djn_perf_counter(const char * string)
    : start_time(djn::get_time_micro())
    , string(string)
{
}

_djn_perf_counter::~_djn_perf_counter()
{
    uint32_t time = djn::get_time_micro() - this->start_time;
    char buffer[32];
    printf("%s : %s us\n", this->string, pretty_number(time,buffer));
}
