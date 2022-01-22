#ifndef LOAD_FILE
#define LOAD_FILE
#include "stdbool.h"

typedef struct {
    size_t init_size;
    size_t new_size;
    size_t change_size;
    bool changed;
} file_information;

char *load_file(char *path);
int save_file(char *path, char *file_to_save);
char *get_full_file_path(char *file_name);

#include "string.h"

file_information
file_info_init(size_t initsize)
{
    file_information this;
    memset(&this, 0, sizeof(file_information));
    this.init_size = initsize;
    this.new_size = 0;
    this.change_size = 0;
    this.changed = false;
    return this;
}

#endif
