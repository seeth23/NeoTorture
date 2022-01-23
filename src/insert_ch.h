#ifndef INSERT_CHAR
#define INSERT_CHAR
#include "file_info.h"

void insert(int *buf, int ch, file_information *p);
void delete(int *buf, file_information *p);
int get_buffer_size();

extern size_t buffer_size;

#endif
