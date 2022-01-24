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

bool save_window(char *file_path);
void init();
void close();
char *convert_buffer(int32_t *src, const file_information p);
new_line_struct *count_for_newlines(char *path_to_read, size_t *new_line_ptr);

int
main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Wrong CL args. File path can't be empty\n");
        exit(EXIT_FAILURE);
    }
    char *file_path = argv[1];

    init();
    mvprintw(40, 0, "You've opened %s", get_full_file_path(file_path));
    int32_t *buf = malloc(4096 * sizeof(int32_t));
    memset(buf, '\0', 4096 * sizeof(int32_t));
    char *file_content = load_file(file_path);

    // COUNT FOR '\n' CHARACTERS
    size_t nline_counter = 0;
    new_line_struct *new_line_map = count_for_newlines(file_content, &nline_counter);

    // Initialize file_information struct
    file_information file_info = file_info_init(strlen(file_content));
    for (size_t i = 0; i < file_info.init_size; i++) {
        buf[i] = file_content[i];
    }

    // Printing file to screen
    int32_t *ptr = buf;
    move(0, 0);
    while (*ptr) {
        addch(*ptr++);
    }
    free(file_content);
    ptr = NULL;

    // MAIN FUNCTION EVENT HANDLER
    handle_input(buf, &file_info, new_line_map);
    close();

    // SAVING FILE
    bool res = save_window(file_path);
    if (res) {
        // Casting int array to char array
        char *final_buf = convert_buffer(buf, file_info);
        printf("Converted buffer: %s\n", final_buf);
        int32_t saved = save_file("test.txt", final_buf);
        printf("Saved %d bytes\n", saved);
        free(final_buf);
    }

    // SOME TECHINAL INFORMATION ABOUT PROCESSES
    printf("Init size: %ld, New size: %ld, Change: %ld, Changed: %s\n", file_info.init_size, file_info.new_size, file_info.change_size, file_info.changed ? "True" : "False");
    /*for (size_t i = 0; i < nline_counter; i++) {
        printf("%ld row has new-line on %ld col\n", new_line_map[i].row, new_line_map[i].column);
        }*/
    //printf("new line counter: %ld\n", nline_counter);
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
    int32_t size = p.new_size;
    if (size == 0) {
        return "";
    }
    char *tmp = malloc(size + 1);
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

new_line_struct *
count_for_newlines(char *path_to_read, size_t *counter)
{
    size_t new_line_count = 0;
    for (size_t i = 0; i < strlen(path_to_read); i++) {
        if (path_to_read[i] == '\n') {
            new_line_count++;
        }
    }

    size_t line_structSIZE = sizeof(new_line_struct) * new_line_count;
    new_line_struct *tmp_pointer = malloc(line_structSIZE);
    if (tmp_pointer == NULL) {
        last_error_log("Failed to allocate memory for struct at main");
        fprintf(stderr, "Failed to alloc memory\n");
        exit(EXIT_FAILURE);
    }

    new_line_struct *start_pos = tmp_pointer;
    /*for (size_t cols = 0, rows = 0, i = 0; i < strlen(path_to_read); cols++) {
        if (path_to_read[cols] == '\n') {
            tmp_pointer->column = cols;
            tmp_pointer->row = rows++;
            tmp_pointer++;
            cols = 0;
        }
        }*/
    size_t row = 0;
    size_t col = 0;
    for (size_t i = 0; i < strlen(path_to_read); i++) {
        if (path_to_read[i] == '\n') {
            tmp_pointer->column = col;
            tmp_pointer->row = row;
            row++;
            col = 0;
            tmp_pointer++;
        }
        col++;
    }
    *counter = new_line_count;
    tmp_pointer = start_pos;
    return tmp_pointer;
}
