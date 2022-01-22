#ifndef FILE_INFO_H
#define FILE_INFO_H
#include "stdbool.h"
#include "stddef.h"
typedef struct {
    size_t init_size;
    size_t new_size;
    size_t change_size;
    bool changed;
} file_information;

file_information file_info_init(size_t initsize);
/*file_information
file_info_init(size_t initsize)
{
    file_information this;
    memset(&this, 0, sizeof(file_information));
    this.init_size = initsize;
    this.new_size = 0;
    this.change_size = 0;
    this.changed = false;
    return this;
    }*/
//void handle_input(int *buf_to_edit, file_information *info_ptr);
#endif
