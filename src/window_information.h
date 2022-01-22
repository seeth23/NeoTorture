#ifndef WINDOW_INFO
#define WINDOW_INFO

#include "stdint.h"

typedef struct window_information {
    int32_t y_height;
    int32_t x_width;
} win_info;

win_info get_wininfo();

#endif
