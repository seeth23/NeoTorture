#include "stdio.h"
#include "stdlib.h"
#include "ncurses.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "time.h"

#include "window_information.h"
#include "cursor.h"
#include "insert_ch.h"
#include "file_info.h"
#include "file_handler.h"
#include "newline.h"

#define F7 271

cursor init_cursor()
{
    cursor tmp_cursor;
    tmp_cursor.prev.y = 0;
    tmp_cursor.prev.x = 0;
    tmp_cursor.current.y = 0;
    tmp_cursor.current.x = 0;
    return tmp_cursor;
}

void shift_to_right(int32_t *buffer_to_edit, file_information *file_info_pointer, struct new_line_list *nline_ptr);
void shift_to_left(int32_t *buffer_to_edit, file_information *file_info_pointer, struct new_line_list *nline_ptr);

void
handle_input(int32_t *buffer_to_edit, file_information *file_info_pointer, struct new_line_list *nline_ptr)
{
    file_info_pointer->cur_cursor = init_cursor();
    int32_t ch;
    //const win_info wInfo = get_wininfo();
    move(file_info_pointer->cur_cursor.prev.y, file_info_pointer->cur_cursor.prev.x);
    while ((ch = getch()) != F7) {
        //file_info_pointer->cur_cursor.prev = file_info_pointer->cur_cursor.current;
        switch(ch) {
        case KEY_UP:
            continue;
        case KEY_DOWN:
            continue;
        case KEY_LEFT:
            if (file_info_pointer->Xbuffer_pos > 0) {
                if (file_info_pointer->cur_cursor.current.x == 0 && file_info_pointer->cur_cursor.current.y > 0) {
                    file_info_pointer->cur_cursor.current.y--;
                    file_info_pointer->cur_cursor.current.x = find_col(nline_ptr, file_info_pointer);
                }
                else if (file_info_pointer->cur_cursor.current.x > 0) {
                    file_info_pointer->cur_cursor.current.x--;
                }
                file_info_pointer->Xbuffer_pos--;
            }
            break;
        case KEY_RIGHT:
            if (file_info_pointer->Xbuffer_pos < file_info_pointer->new_size) {
                if (buffer_to_edit[file_info_pointer->Xbuffer_pos] == '\n') {
                    file_info_pointer->cur_cursor.current.y++;
                    file_info_pointer->cur_cursor.current.x = 0;
                } else
                    file_info_pointer->cur_cursor.current.x++;
                file_info_pointer->Xbuffer_pos++;
            }
            break;
        case KEY_BACKSPACE:
            if (file_info_pointer->Xbuffer_pos > 0 && file_info_pointer->cur_cursor.current.x > 0) {
                if (buffer_to_edit[file_info_pointer->Xbuffer_pos] == '\n')
                    //                    break;

                // shifts \n to the left
                shift_to_left(buffer_to_edit, file_info_pointer, nline_ptr);

                file_info_pointer->cur_cursor.current.x--;
                file_info_pointer->Xbuffer_pos--;
                delete(buffer_to_edit, file_info_pointer);
            }
            break;
        default:
            if (ch == '\n') {
                update_line_list(nline_ptr, file_info_pointer);
                file_info_pointer->cur_cursor.current.y++;
                file_info_pointer->cur_cursor.current.x = 0;
            } else
                file_info_pointer->cur_cursor.current.x++;
            insert(buffer_to_edit, ch, file_info_pointer);

            // shifts \n to the right
            shift_to_right(buffer_to_edit, file_info_pointer, nline_ptr);

            file_info_pointer->Xbuffer_pos++;
            break;
        }

        clear();
        for (int i = 0; i < file_info_pointer->new_size; i++) {
            if (buffer_to_edit[i] == '\t') {
                addch(' ');
                continue;
            }
            if (buffer_to_edit[i] == '\n') {
                //addch('N');
                //continue;
            }
            addch(buffer_to_edit[i]);
        }

        // information
        mvprintw(40, 90, "Current buffer: initsize: %ld newsize: %ld XBuffer_pos: %d", file_info_pointer->init_size, file_info_pointer->new_size, file_info_pointer->Xbuffer_pos);
        mvprintw(41, 90, "Current buffer: cursor x: %d cursor y: %d newsize: %ld elements after cursor: %ld", file_info_pointer->cur_cursor.current.x, file_info_pointer->cur_cursor.current.y, file_info_pointer->new_size, file_info_pointer->new_size - file_info_pointer->cur_cursor.current.x);
        move(file_info_pointer->cur_cursor.current.y, file_info_pointer->cur_cursor.current.x);
    }
}

void
shift_to_right(int32_t *buffer_to_edit, file_information *file_info_pointer, struct new_line_list *nline_ptr)
{
    if (buffer_to_edit[file_info_pointer->Xbuffer_pos + 1] == '\n') {
        struct new_line_list *head_tmp = nline_ptr;
        while (head_tmp->next != NULL) {
            if (head_tmp->line.row == file_info_pointer->cur_cursor.current.y) {
                head_tmp->line.column++;
                break;
            }
            head_tmp = head_tmp->next;
        }
    } else {
        struct new_line_list *head_tmp = nline_ptr;
        while (head_tmp->next != NULL) {
            if (head_tmp->line.row == file_info_pointer->cur_cursor.current.y) {
                head_tmp->line.column++;
                break;
            }
            head_tmp = head_tmp->next;
        }
    }
}

void
shift_to_left(int32_t *buffer_to_edit, file_information *file_info_pointer, struct new_line_list *nline_ptr)
{
    struct new_line_list *head_tmp = nline_ptr;
    while (head_tmp->next != NULL) {
        if (head_tmp->line.row == file_info_pointer->cur_cursor.current.y) {
            head_tmp->line.column--;
            break;
        }
        head_tmp = head_tmp->next;
    }
}
