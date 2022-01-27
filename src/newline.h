#ifndef NEW_LINE_H
#define NEW_LINE_H

#include "file_handler.h"
#include "file_info.h"

void update_line_list(struct new_line_list *list_ptr, file_information *info_ptr);
struct new_line_list *init_line_list();
int32_t find_col(struct new_line_list *list_finder, file_information *info_ptr);

#endif
