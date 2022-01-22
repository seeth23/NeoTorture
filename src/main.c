#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
#include "ncurses.h"

#include "file_handler.h"
#include "key_handle.h"
#include "window_information.h"
#include "insert_ch.h"

bool save_window(char *file_path);
void init();
void close();
char *convert_buffer(int32_t *src);

int
main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Wrong CL args. File path can't be empty\n");
        exit(EXIT_FAILURE);
    }
    char *file_path = argv[1];

    init();
    mvprintw(0, 0, "You've opened %s", get_full_file_path(file_path));
    int32_t *buf = malloc(4096 * sizeof(int32_t));

    char *file_content = load_file(file_path);

    //    addstr(file_content);
    //buffer_size = strlen(file_content);

    for (size_t i = 0; i < strlen(file_content); i++) {
        buf[i] = file_content[i];
    }
    int32_t *ptr = buf;
    move(1, 0);

    while (*ptr) {
        addch(*ptr++);
    }
    free(file_content);

    handle_input(buf);
    close();

    bool res = save_window(file_path);
    if (res) {
        // Casting int array to char array
        char *final_buf = convert_buffer(buf);
        printf("Converted buffer: %s\n", final_buf);
        int32_t saved = save_file("test.txt", final_buf);
        printf("Saved %d bytes\n", saved);
        free(final_buf);
    }
    free(buf);
    return 0;
}


void
init()
{
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    cbreak();
}

void
close()
{
    endwin();
}

char *
convert_buffer(int32_t *src)
{
    int32_t size = get_buffer_size();

    char *tmp = malloc(size + 1);
    if (size == 0) {
        return "";
    }
    for (size_t i = 0; i < size; i++) {
        tmp[i] = (char)src[i];
    }

    tmp[size] = '\0';
    return tmp;
}

bool
save_window(char *file_path)
{
    printf("Save %s?\t y/n \n", get_full_file_path(file_path));

    char choose;
    bool do_save = false;
    choose = getchar();
    while (1) {
        if (choose == 'y' || choose == 'n')
            break;

        choose = getchar();
    }
    switch (choose) {
    case 'y':
        do_save = true;
        break;
    case 'n':
        do_save = false;
        break;
    }
    return do_save;
}
