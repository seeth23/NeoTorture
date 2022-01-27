#define _GNU_SOURCE
#include "ncurses.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stddef.h"
#include "string.h"

#include "current_time.h"

void last_error_log(char *l_err);

static char *error_format(char *part1, char *part2)
{
    char *tmp;
    if (0 > asprintf(&tmp,"%s %s", part1, part2)) {
        endwin();
        last_error_log("Failed to format string");
        exit(EXIT_FAILURE);
    }
    return tmp;
}

char *
get_full_file_path(char *file_name)
{
    char fullpath[100];
    char *ptr = realpath(file_name, fullpath);
    if (ptr == NULL) {
        //endwin();
        //fprintf(stderr, "Unable to find file\n");
        return file_name;
        //exit(EXIT_FAILURE);
    }
    return ptr;
}

char *
load_file(char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        endwin();
        fprintf(stderr, "Failed to open %s\n", get_full_file_path(path));
        char *err = error_format("Failed to open", get_full_file_path(path));
        last_error_log(err);
        free(err);
        exit(EXIT_FAILURE);
    }

    size_t file_size;
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    if (file_size == 0) {
        fclose(file);
        char *returned_empty_string = malloc(sizeof(char));
        memset(returned_empty_string, 0, sizeof(char));
        return returned_empty_string;
    }

    size_t file_buffer_SIZE = file_size * sizeof(char) + 1;
    //printf("file_buffer_SIZE: %ld\n", file_size);
    char *file_buffer = malloc(file_buffer_SIZE);

    if (file_buffer == NULL) {
        endwin();
        last_error_log("Failed to allocate memory for buffer");
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    int32_t symbol;
    while ((symbol = fread(file_buffer, 1, file_size, file)) > 0)
        ;

    file_buffer[file_buffer_SIZE] = '\0';

    if (strlen(file_buffer) != file_size) {
        endwin();
        last_error_log("Failed to read file properly");
        fprintf(stderr, "Failed to read %s\n", path);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    return file_buffer;
}

static void
edit_time_log();

int32_t
save_file(char *path, char *buffer_to_save)
{
    FILE *file_to_save = fopen(path, "w");
    if (file_to_save == NULL) {
        fclose(file_to_save);
        char *err = error_format("Failed to save", get_full_file_path(path));
        last_error_log(err);
        fprintf(stderr, "Failed to save file\n");
        free(err);
        exit(EXIT_FAILURE);
    }

    if (strlen(buffer_to_save) == 0) {
        fclose(file_to_save);
        fprintf(stderr, "Nothing to write, saving...\n");
        exit(EXIT_FAILURE);
    }

    int32_t wrote = fwrite(buffer_to_save, 1, strlen(buffer_to_save), file_to_save);

    fclose(file_to_save);
    edit_time_log();
    return wrote;
}

static void
edit_time_log()
{
    FILE *file = fopen("logs/save_log.txt", "a+");
    fprintf(file, "Last edit: %s", get_current_time());
    fclose(file);
}

void
last_error_log(char *l_err)
{
    FILE *file = fopen("logs/err_log.txt", "a+");
    fprintf(file, "Last error: %s at %s", l_err, get_current_time());
    fclose(file);
}
