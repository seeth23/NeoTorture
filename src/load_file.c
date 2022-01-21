#include "ncurses.h"

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stddef.h"
#include "string.h"

char *
load_file(char *path)
{
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open the file\n");
        exit(EXIT_FAILURE);
    }
    
    size_t file_size;
    fseek(file, 0L, SEEK_END);
    file_size = ftell(file);
    rewind(file);
    
    // mvprintw(30, 30, "File size: %ld", file_size);

    //move(0, 0);
    size_t file_buffer_SIZE = file_size * sizeof(char) + 1;
    char *file_buffer = malloc(file_buffer_SIZE);

    if (file_buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    
    int symbol;
    while ((symbol = fread(file_buffer, 1, file_size, file)) > 0)
        ;

    file_buffer[file_buffer_SIZE] = '\0';
    
    if (strlen(file_buffer) != file_size) {
        fprintf(stderr, "Failed to read %s\n", path);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    
    return file_buffer;
}

int
save_file(char *path, char *buffer_to_save)
{
    FILE *file_to_save = fopen(path, "w");
    
    if (file_to_save == NULL) {
        fprintf(stderr, "Failed to save file\n");
        exit(EXIT_FAILURE);
    }
    int wrote = fwrite(buffer_to_save, 1, strlen(buffer_to_save + 1), file_to_save);
    
    fclose(file_to_save);
    return wrote;
}
