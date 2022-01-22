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
#include "current_time.h"
#include "file_info.h"

typedef struct {
    size_t column;
    int32_t null_term;
} new_line_struct;

bool save_window(char *file_path);
void init();
void close();
char *convert_buffer(int32_t *src, const file_information p);

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

    // COUNT FOR '\n' CHARACTERS
    size_t new_line_count = 0;
    for (size_t i = 0; i < strlen(file_content); i++) {
        if (file_content[i] == '\n') {
            new_line_count++;
        }
    }
    size_t line_structSIZE = sizeof(new_line_struct) * new_line_count;
    new_line_struct *new_line_map = malloc(line_structSIZE);
    memset(new_line_map, 0, line_structSIZE);
    if (new_line_map == NULL) {
        fprintf(stderr, "Failed to alloc memory\n");
        exit(EXIT_FAILURE);
    }
    int j = 0;
    for (size_t i = 0; i < strlen(file_content); i++) {
        if (file_content[i] == '\n') {
            new_line_map[j].column = i;
            new_line_map[j++].null_term = '\n';
        }
    }

    // Initialize file_information struct
    file_information file_info = file_info_init(strlen(file_content));

    for (size_t i = 0; i < file_info.init_size; i++) {
        buf[i] = file_content[i];
    }
    int32_t *ptr = buf;
    move(1, 0);

    while (*ptr) {
        addch(*ptr++);
    }

    free(file_content);
    ptr = NULL;

    // MAIN FUNCTION
    handle_input(buf, &file_info);
    close();

    bool res = save_window(file_path);
    if (res) {
        // Casting int array to char array
        char *final_buf = convert_buffer(buf, file_info);
        printf("Converted buffer: %s\n", final_buf);
        int32_t saved = save_file("test.txt", final_buf);
        printf("Saved %d bytes\n", saved);
        free(final_buf);
    }
    printf("\n");
    printf("SIZEOF STRUCTURE: %ld\n", line_structSIZE);
    for (size_t i = 0; i < new_line_count; i++) {
        printf("%ld row has new-line on %ld col\n", i, new_line_map[i].column);
    }
    printf("Change between init_size and new_size: %ld\n", file_info.change_size);
    free(buf);
    free(new_line_map);
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
convert_buffer(int32_t *src, const file_information p)
{
    //int32_t size = get_buffer_size();
    int32_t size = p.new_size;
    char *tmp = malloc(size + 1);
    if (size == 0) {
        return " ";
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
