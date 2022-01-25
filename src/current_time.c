#include "time.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

char *
get_current_time()
{
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char *time_buffer = asctime(timeinfo);
    return time_buffer;
}
