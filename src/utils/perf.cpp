#include "perf.h"
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