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
    printf("file size: %ld\n", file_size);
    char *file_buffer = malloc(file_size * sizeof(char));

    if (file_buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    int symbol;
    while ((symbol = fread(file_buffer, 1, file_size, file)) > 0)
        ;

    if (strlen(file_buffer) != file_size) {
        fprintf(stderr, "Failed to read %s\n", path);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    
    return file_buffer;
}
