#include "stdio.h"
#include "stdlib.h"
#include "ncurses.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"

#define F7 271

#include "window_information.h"

#include "insert_ch.h"

void
handle_input(int *buffer_to_edit)
{
    int x = 0;
    int y = 0;
    int c;
    int32_t pos = 0;
    const win_info wInfo = get_wininfo();
    
    c = getch();
    do {
        switch(c) {
        case KEY_DOWN:
            if (y < wInfo.y_height)
                y++;
            break;
        case KEY_UP:
            if (y > 0)
                y--;
            break;
        case KEY_LEFT:
            if (x > 0)
                x--;
            break;
        case KEY_RIGHT:
            if (x < wInfo.x_width)
                x++;
            break;
        case KEY_BACKSPACE:
            delch();
            x--;
            delete(buffer_to_edit);
            break;
        case F7:
            
            break;
        default:
            insert(buffer_to_edit, c);
            addch(buffer_to_edit[pos++]);
            x++;
            break;
        }
        move(y, x);
        refresh();
    } while ((c = getch()) != F7);
}
