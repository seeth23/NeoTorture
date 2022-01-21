#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "ncurses.h"

#include "load_file.h"
#include "key_handle.h"
#include "window_information.h"

void init();
void close();

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Wrong CL args. File path can't be empty\n");
        exit(EXIT_FAILURE);
    }
    
    char *file_path = argv[1];

    init();
    
    win_info winYX = get_wininfo();
    
    char *file_content = load_file(file_path);
    
    //    printw("%s", file_content);
    addstr(file_content);
    move(0, 0);
    
    handle_input(0);

    char buf[1024][1024];
    //int n = mvinch(0, 0);
    
    getch();
    close();
    //printf("height: %d, width %d\n", winYX.y_height, winYX.x_width);
    int i, j;

    for (i = 0; i < 50; i++) {
        for (j = 0; j < 50; j++) {
            buf[i][j] = mvinch(i, j);
        }
    }

    buf[49][49] = '\0';
    
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            printf("%c", buf[i][j]);
        }
        printf("\n");
    }
    
    free(file_content);
    return 0;
}


void init()
{
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    cbreak();
}

void close()
{
    endwin();
}
