#ifndef LOAD_FILE
#define LOAD_FILE
#include "stdbool.h"

char *load_file(char *path);
int save_file(char *path, char *file_to_save);
char *get_full_file_path(char *file_name);

#endif
