#include "ncurses.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stddef.h"
#include "string.h"

#include "current_time.h"

char *
get_full_file_path(char *file_name)
{
    char fullpath[100];
    char *ptr = realpath(file_name, fullpath);
    if (ptr == NULL) {
        fprintf(stderr, "Unable to find file\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

char *
load_file(char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open %s\n", get_full_file_path(path));
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

    int32_t file_buffer_SIZE = file_size * sizeof(char);
    char *file_buffer = malloc(file_buffer_SIZE);

    if (file_buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    int32_t symbol;
    while ((symbol = fread(file_buffer, 1, file_size, file)) > 0)
        ;

    file_buffer[file_buffer_SIZE] = '\0';

    if (strlen(file_buffer) != file_size) {
        endwin();
        printf("file_size: %ld\tstrlen(file_buffer): %ld\n", file_size, strlen(file_buffer));
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
        fprintf(stderr, "Failed to save file\n");
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
