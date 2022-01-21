#include "ncurses.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "window_information.h"

win_info get_wininfo()
{
    win_info wInfo;
    memset(&wInfo, 0, sizeof(win_info));
    getmaxyx(stdscr, wInfo.y_height, wInfo.x_width);
    return wInfo;
}
