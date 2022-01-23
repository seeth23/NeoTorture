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
    char cursor_log[CURSOR_LOG_SIZE];
    memset(cursor_log, 0, CURSOR_LOG_SIZE);

    file_info_pointer->cur_cursor = init_cursor();
    int *pbuf = buffer_to_edit;
    int32_t ch;

    const win_info wInfo = get_wininfo();
    //FILE *log_file = fopen("logs/cursor_log.txt", "w");

    move(file_info_pointer->cur_cursor.prev.y, file_info_pointer->cur_cursor.prev.x);
    while ((ch = getch()) != F7) {
        file_info_pointer->cur_cursor.prev = file_info_pointer->cur_cursor.current;
        switch(ch) {
        case KEY_DOWN:
            if (file_info_pointer->cur_cursor.current.y < wInfo.y_height) {
                file_info_pointer->cur_cursor.current.y++;
            }
            break;
        case KEY_UP:
            if (file_info_pointer->cur_cursor.current.y > 0)
                file_info_pointer->cur_cursor.current.y--;
            break;
        case KEY_LEFT:
            if (file_info_pointer->cur_cursor.current.x > 0) {
                file_info_pointer->cur_cursor.current.x--;
            }
            break;
        case KEY_RIGHT:
            if (file_info_pointer->cur_cursor.current.x < wInfo.x_width) {
                file_info_pointer->cur_cursor.current.x++;
            }
            break;
        case KEY_BACKSPACE:
            if (file_info_pointer->cur_cursor.current.x > 0) {
                //delch();

                file_info_pointer->cur_cursor.current.x--;
                delete(buffer_to_edit, file_info_pointer);
            }
            break;
        default:
            insert(pbuf, ch, file_info_pointer);
            file_info_pointer->cur_cursor.current.x++;
            //addch((char)buffer_to_edit[file_info_pointer->cur_cursor.current.x++]);
            break;
        }
        //mvprintw(40, 100, "Current cursor pos: y: %d, x: %d", file_info_pointer->cur_cursor.current.y, file_info_pointer->cur_cursor.current.x);
        refresh();
        clear();
        for (int i = 0; i < file_info_pointer->new_size; i++) {
            addch(buffer_to_edit[i]);
        }
        mvprintw(40, 100, "Current buffer: initsize: %ld newsize: %ld", file_info_pointer->init_size, file_info_pointer->new_size);
        mvprintw(41, 100, "Current buffer: cursor x: %d newsize: %ld elements after cursor: %ld", file_info_pointer->cur_cursor.current.x, file_info_pointer->new_size, file_info_pointer->new_size - file_info_pointer->cur_cursor.current.x);

        move(file_info_pointer->cur_cursor.current.y, file_info_pointer->cur_cursor.current.x);


    }
    //fclose(log_file);
}
