#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "ncurses.h"

#include "load_file.h"
#include "key_handle.h"
#include "window_information.h"
#include "insert_ch.h"

void init();
void close();
char *convert_buffer(int *src);

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Wrong CL args. File path can't be empty\n");
        exit(EXIT_FAILURE);
    }
    
    char *file_path = argv[1];

    init();

    int *buf = malloc(4096 * sizeof(int));
    
    char *file_content = load_file(file_path);
    
    addstr(file_content);
    move(0, 0);
    
    handle_input(buf);
            
    getch();
    close();
    
    // Casting int array to char array for saving into file
    char *final_buf = convert_buffer(buf);
    printf("converted buffer: %s\n", final_buf);
    
    save_file("test.txt", final_buf);
    
    free(file_content);
    free(buf);
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

char *
convert_buffer(int *src)
{
    int size = get_buffer_size();
    char *tmp = malloc(size + 1);

    for (int i = 0; i < size; i++) {
        tmp[i] = (char)src[i];
    }
            
    tmp[size + 1] = '\0';
    return tmp;
}
