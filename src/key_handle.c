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

#define CURSOR_LOG_SIZE 4096

void
handle_input(int32_t *buffer_to_edit, file_information *file_info_pointer, struct new_line_list *nline_ptr)
{
    //char cursor_log[CURSOR_LOG_SIZE];
    //memset(cursor_log, 0, CURSOR_LOG_SIZE);
    file_info_pointer->cur_cursor = init_cursor();
    int32_t ch;
    //const win_info wInfo = get_wininfo();
    //FILE *log_file = fopen("logs/cursor_log.txt", "w");
    move(file_info_pointer->cur_cursor.prev.y, file_info_pointer->cur_cursor.prev.x);
    //new_line_struct *tmp = NULL;
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
                    //file_info_pointer->cur_cursor.current.x = new_line_pointer[file_info_pointer->cur_cursor.current.y - 1].column - 1;
                    file_info_pointer->cur_cursor.current.x = find_col(nline_ptr, file_info_pointer);
                    file_info_pointer->cur_cursor.current.y--;
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
            if (file_info_pointer->Xbuffer_pos > 0) {
                file_info_pointer->cur_cursor.current.x--;
                file_info_pointer->Xbuffer_pos--;
                delete(buffer_to_edit, file_info_pointer);
            }
            break;
        default:
            if (ch == '\n') {
                //REMOVE
                //tmp = update_newlines(new_line_pointer, file_info_pointer);
                update_line_list(nline_ptr, file_info_pointer);
                file_info_pointer->cur_cursor.current.y++;
                file_info_pointer->cur_cursor.current.x = 0;
            } else
                file_info_pointer->cur_cursor.current.x++;
            insert(buffer_to_edit, ch, file_info_pointer);
            file_info_pointer->Xbuffer_pos++;
            break;
        }
        //mvprintw(40, 100, "Current cursor pos: y: %d, x: %d", file_info_pointer->cur_cursor.current.y, file_info_pointer->cur_cursor.current.x);
        clear();
        for (int i = 0; i < file_info_pointer->new_size; i++) {
            if (buffer_to_edit[i] == '\t') {
                addch(' ');
                continue;
            }
            addch(buffer_to_edit[i]);
        }
        //REMOVE TOMORROW
        //if (tmp != NULL)
        //    mvprintw(46, 100, "ptr->row: %ld ptr->column: %ld", tmp->row, tmp->column);
        mvprintw(40, 100, "Current buffer: initsize: %ld newsize: %ld XBuffer_pos: %d", file_info_pointer->init_size, file_info_pointer->new_size, file_info_pointer->Xbuffer_pos);
        mvprintw(41, 100, "Current buffer: cursor x: %d newsize: %ld elements after cursor: %ld", file_info_pointer->cur_cursor.current.x, file_info_pointer->new_size, file_info_pointer->new_size - file_info_pointer->cur_cursor.current.x);
        move(file_info_pointer->cur_cursor.current.y, file_info_pointer->cur_cursor.current.x);
    }
    //fclose(log_file);
}
