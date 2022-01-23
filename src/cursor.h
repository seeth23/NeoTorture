#ifndef CURSOR_H
#define CURSOR_H

#include "stddef.h"
#include "stdlib.h"
typedef struct {
    int32_t y;
    int32_t x;
} cursor_y_x;

typedef struct {
    cursor_y_x prev;
    cursor_y_x current;
} cursor;

#endif
