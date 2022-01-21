#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "ncurses.h"

#include "load_file.h"

int main(int argc, char **argv)
{
    if (argc > 2) {
        fprintf(stderr, "Wrong CLI arguments\n");
        exit(EXIT_FAILURE);
    }
    
    char *buf = load_file(argv[1]);
    
    printf("*buf: \n%s\n", buf);

    
    return 0;
}
