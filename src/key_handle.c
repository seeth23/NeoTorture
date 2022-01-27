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

static void shift_to_right(int32_t *buffer_to_edit, file_information *file_info_pointer, struct new_line_list *nline_ptr);
static void shift_to_left(int32_t *buffer_to_edit, file_information *file_info_pointer, struct new_line_list *nline_ptr);
static void move_nodes(file_information *file_info_pointer, struct new_line_list *nline_ptr, size_t eol);
static size_t get_last_line(file_information *file_info_pointer, struct new_line_list *nline_ptr);
static size_t get_EOL(file_information *file_info_pointer, struct new_line_list *nline_ptr);

void
handle_input(int32_t *buffer_to_edit, file_information *file_info_pointer, struct new_line_list *nline_ptr)
{
    file_info_pointer->cur_cursor = init_cursor();
    int32_t ch;
    //const win_info wInfo = get_wininfo();
    size_t last_line_index = 0;
    size_t EOL = 0;
    move(file_info_pointer->cur_cursor.prev.y, file_info_pointer->cur_cursor.prev.x);

    while ((ch = getch()) != F7) {
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
                // shifts \n to the left
                shift_to_left(buffer_to_edit, file_info_pointer, nline_ptr);
                file_info_pointer->cur_cursor.current.x--;
                file_info_pointer->Xbuffer_pos--;
                delete(buffer_to_edit, file_info_pointer);
            }
            break;
        default:
            if (ch == '\n') {
                if (file_info_pointer->cur_cursor.current.y >= 0 && file_info_pointer->cur_cursor.current.y < last_line_index
                    && EOL != -1) {
                    move_nodes(file_info_pointer, nline_ptr, EOL);
                } else if (file_info_pointer->cur_cursor.current.y == last_line_index && EOL == -1)
                    update_line_list(nline_ptr, file_info_pointer);
                file_info_pointer->cur_cursor.current.y++;
                file_info_pointer->cur_cursor.current.x = 0;

            } else {
                file_info_pointer->cur_cursor.current.x++;
                shift_to_right(buffer_to_edit, file_info_pointer, nline_ptr);
            }
            insert(buffer_to_edit, ch, file_info_pointer);
            file_info_pointer->Xbuffer_pos++;
            break;
        }

        clear();
        for (int i = 0; i < file_info_pointer->new_size; i++) {
            if (buffer_to_edit[i] == '\t') {
                addch(' ');
                continue;
            }
            addch(buffer_to_edit[i]);
        }

        // Geting index of last line
        last_line_index = get_last_line(file_info_pointer, nline_ptr);
        // Geting end of current line index
        EOL = get_EOL(file_info_pointer, nline_ptr);
        // information
        mvprintw(40, 90, "Current buffer: initsize: %ld newsize: %ld XBuffer_pos: %d", file_info_pointer->init_size, file_info_pointer->new_size, file_info_pointer->Xbuffer_pos);
        mvprintw(41, 90, "Current buffer: cursor x: %d cursor y: %d newsize: %ld", file_info_pointer->cur_cursor.current.x, file_info_pointer->cur_cursor.current.y, file_info_pointer->new_size);
        mvprintw(42, 90, "Last line: %ld", last_line_index);
        mvprintw(43, 90, "EOL: %ld", EOL);
        mvprintw(44, 90, "First list data: col: %ld row: %ld", nline_ptr->line.column, nline_ptr->line.row);
        move(file_info_pointer->cur_cursor.current.y, file_info_pointer->cur_cursor.current.x);
    }
}

static void
shift_to_right(int32_t *buffer_to_edit, file_information *file_info_pointer, struct new_line_list *nline_ptr)
{
    struct new_line_list *head_tmp = nline_ptr;
    while (head_tmp->next != NULL) {
        if (head_tmp->line.row == file_info_pointer->cur_cursor.current.y) {
            head_tmp->line.column++;
            break;
        }
        head_tmp = head_tmp->next;
    }
}

static void
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

static void
move_nodes(file_information *file_info_pointer, struct new_line_list *nline_ptr, size_t eol)
{
    struct new_line_list *prev = nline_ptr;
    struct new_line_list *finder = prev->next;
    while (finder != NULL) {
        if (finder->line.row == file_info_pointer->cur_cursor.current.y) {
            struct new_line_list *found = finder;

            struct new_line_list *tmp = init_line_list();
            tmp->line.column = file_info_pointer->cur_cursor.current.x;
            tmp->line.row = found->line.row;

            prev->next = tmp;
            tmp->next = found;

            found->line.column = eol - file_info_pointer->cur_cursor.current.x;

            struct new_line_list *beg = prev->next;
            while (beg->next != NULL) {
                beg->next->line.row++;
                beg = beg->next;
            }
            break;
        }
        prev = prev->next;
        finder = prev->next;
    }
}

static size_t
get_last_line(file_information *file_info_pointer, struct new_line_list *nline_ptr)
{
    struct new_line_list *head = nline_ptr;
    size_t counter = 0;
    while (head->next != NULL) {
        head = head->next;
        counter++;
    }
    return counter;
}

static size_t
get_EOL(file_information *file_info_pointer, struct new_line_list *nline_ptr)
{
    struct new_line_list *hd = nline_ptr;
    size_t local_end_ofline = 0;
    while (hd->next != NULL) {
        if (hd->line.row == file_info_pointer->cur_cursor.current.y) {
            local_end_ofline = hd->line.column;
            return local_end_ofline;
        }
        hd = hd->next;
    }
    return -1;
}
