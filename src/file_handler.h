#ifndef LOAD_FILE
#define LOAD_FILE

#include "stdbool.h"

char *load_file(char *path);
int save_file(char *path, char *file_to_save);
char *get_full_file_path(char *file_name);
void last_error_log(char *err);

typedef struct {
    size_t column;
    size_t row;
} new_line_struct;

struct new_line_list{
    struct new_line_list *next;
    new_line_struct line;
};

#endif
