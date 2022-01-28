#ifndef KEY_HANDLE
#define KEY_HANDLE
#include "file_info.h"
#include "file_handler.h"
#include "ncurses.h"

void handle_input(int *buf_to_edit, file_information *info_ptr, struct new_line_list *nline_ptr, WINDOW *win_info);

#endif
