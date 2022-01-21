#include "stdio.h"
#include "stdlib.h"
#include "ncurses.h"
#include "string.h"
#include "stdbool.h"

#include "window_information.h"

void
handle_input(int ch)
{
    int x = 0;
    int y = 0;
    int c;
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
            x--;
            delch();
            break;
        default:
            addch(c);
            x++;
            break;
        }
        move(y, x);
        refresh();
    } while ((c = getch()) != 'q');
}

