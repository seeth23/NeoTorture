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
#include "newline.h"

bool save_window(char *file_path);
void init();
void close();
char *convert_buffer(int32_t *src, const file_information p);
void free_list(struct new_line_list *head);
struct new_line_list* count_for_newlines(char *path_to_read);

int
main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Wrong CL args. File path can't be empty\n");
        exit(EXIT_FAILURE);
    }

    char *file_path = argv[1];

    mvprintw(40, 0, "You've opened %s", get_full_file_path(file_path));
    int32_t *buf = malloc(4096 * sizeof(int32_t));
    memset(buf, 0, 4096 * sizeof(int32_t));
    char *file_content = load_file(file_path);

    // Initialize file_information struct
    file_information file_info = file_info_init(strlen(file_content));
    for (size_t i = 0; i < file_info.init_size; i++) {
        buf[i] = file_content[i];
    }

    init();
    // Count for new lines. if 0 returns NULL
    struct new_line_list *new_lines = count_for_newlines(file_content);
    struct new_line_list *check = new_lines;
    if (check == NULL)
        check = init_line_list();

    // Printing file to screen
    int32_t *ptr = buf;
    move(0, 0);
    while (*ptr) {
        addch(*ptr++);
    }
    ptr = NULL;
    free(file_content);

    // MAIN FUNCTION EVENT HANDLER
    handle_input(buf, &file_info, check);
    close();

    // SAVING FILE
    bool res = save_window(file_path);
    if (res) {
        // Casting int array to char array
        char *final_buf = convert_buffer(buf, file_info);
        //printf("Converted buffer: %s\n", final_buf);
        int32_t saved = save_file("test.txt", final_buf);
        printf("Saved %d bytes\n", saved);
        free(final_buf);
    }

    // SOME TECHINAL INFORMATION ABOUT PROCESSES
    struct new_line_list *begin = check;
    /*while (check->next != NULL) {
        printf("check col: %ld check row: %ld\n", check->line.column, check->line.row);
        check = check->next;
        }*/
    printf("Init size: %ld, New size: %ld, Change: %ld, Changed: %s\n", file_info.init_size, file_info.new_size, file_info.change_size, file_info.changed ? "True" : "False");
    free(buf);
    free_list(begin);
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

struct new_line_list*
count_for_newlines(char *path_to_read)
{
    struct new_line_list *ptr = NULL;
    size_t tmp_counter = 0;
    for (size_t i = 0; i < strlen(path_to_read); i++) {
        if (path_to_read[i] == '\n') {
            tmp_counter++;
        }
    }
    if (tmp_counter == 0) {
        return NULL;
    }
    struct new_line_list *tmp = malloc(sizeof(struct new_line_list));
    ptr = tmp;
    struct new_line_list *begin = tmp;
    if (ptr) {
        while (tmp_counter--) {
            ptr->next = init_line_list();
            ptr = ptr->next;
        }
    } else {
        endwin();
        fprintf(stderr, "Failed to make it with ptr\n");
        exit(EXIT_FAILURE);
    }
    ptr->next = NULL;
    ptr = begin;
    size_t rows = 0, cols = 0;
    for (size_t i = 0; i < strlen(path_to_read) + 1; i++) {
        if (path_to_read[i] == '\n') {
            ptr->line.column = cols;
            ptr->line.row = rows;
            ptr = ptr->next;
            rows++;
            cols = 0;
            continue;
        }
        cols++;
    }
    ptr = NULL;
    return begin;
}

void
free_list(struct new_line_list *head)
{
    struct new_line_list *first = NULL;
    while (head->next != NULL) {
        first = head;
        head = head->next;
        free(first);
    }
}
