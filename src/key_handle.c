#include "stdio.h"
#include "stdlib.h"
#include "ncurses.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "time.h"

#define F7 271

#include "window_information.h"
#include "cursor.h"
#include "insert_ch.h"
#include "file_info.h"
//#include "file_handler.h"

cursor init_cursor()
{
    cursor tmp_cursor;
    tmp_cursor.prev.y = 1;
    tmp_cursor.prev.x = 0;
    tmp_cursor.current.y = 1;
    tmp_cursor.current.x = 0;

    return tmp_cursor;
}

#define CURSOR_LOG_SIZE 4096

void
handle_input(int32_t *buffer_to_edit, file_information *file_info_pointer)
{
    cursor cur_pos = init_cursor();
    char cursor_log[CURSOR_LOG_SIZE];
    memset(cursor_log, 0, CURSOR_LOG_SIZE);

    int ch;

    //int32_t *pointer_to_buffer = buffer_to_edit;
    //int32_t pos = 0;
    const win_info wInfo = get_wininfo();

    FILE *log_file = fopen("logs/cursor_log.txt", "w");

    move(cur_pos.prev.y, cur_pos.prev.x);
    //ch = getch();
    while ((ch = getch()) != F7) {
        cur_pos.prev = cur_pos.current;
        switch(ch) {
        case KEY_DOWN:
            if (cur_pos.current.y < wInfo.y_height) {
                cur_pos.current.y++;
            }
            break;
        case KEY_UP:
            if (cur_pos.current.y > 0)
                cur_pos.current.y--;
            break;
        case KEY_LEFT:
            if (cur_pos.current.x > 0) {
                cur_pos.current.x--;
            }
            break;
        case KEY_RIGHT:
            if (cur_pos.current.x < wInfo.x_width) {
                cur_pos.current.x++;
            }
            break;
        case KEY_BACKSPACE:
            if (cur_pos.current.x > 0) {
                delch();
                //pos--;
                delete(buffer_to_edit, file_info_pointer);
                cur_pos.current.x--;
            }
            break;
        default:
            insert(buffer_to_edit, ch, file_info_pointer);
            addch((char)buffer_to_edit[cur_pos.current.x++]);
            //cur_pos.current.x++;
            break;
        }
        move(cur_pos.current.y, cur_pos.current.x);

        fprintf(log_file, "PREV y: %d x: %d\nCURRENT y: %d x: %d\n", cur_pos.prev.y, cur_pos.prev.x, cur_pos.current.y, cur_pos.current.x);
        refresh();
    }
    //    pointer_to_buffer = NULL;
    fclose(log_file);
}
