#ifndef FILE_INFO_H
#define FILE_INFO_H

#include "stdbool.h"
#include "stddef.h"
#include "cursor.h"
typedef struct {
    size_t init_size;
    size_t new_size;
    size_t change_size;
    bool changed;
    cursor cur_cursor;
    int32_t Xbuffer_pos;
} file_information;

file_information file_info_init(size_t initsize);

#endif
