#include "stddef.h"
#include "stdlib.h"
#include "ncurses.h"
#include "string.h"

#include "file_info.h"
static file_information *s_global_info = NULL;
#define MAX_SIZE 4096
size_t buffer_size = 0;
//size_t cursor_position = 0;

static int push(int *buf, int ch);
static int pop(int *buf);

static void change_file_information(file_information *u)
{
    u->change_size = u->new_size - u->init_size;
    u->changed = true;
    s_global_info = u;
}

int get_buffer_size()
{
    return buffer_size;
}

void
insert(int *buf, char ch, file_information *p)
{
    s_global_info = p;
    int res = push(buf, ch);
    //cursor_position = cur_pos;
    change_file_information(p);
    if (res == -1) {
        mvprintw(40, 100, "Failed to add %c to buffer", ch);
    }
}

void
delete(int *buf, file_information *p)
{
    s_global_info = p;
    change_file_information(p);
    pop(buf);
    //mvprintw(40, 100, "deleted %c", ch);
}

static int
push(int *buf, int ch)
{
    if (MAX_SIZE - buffer_size < 10) {
        mvprintw(40, 100, "buffer size is almost full: %ld/%d", buffer_size, MAX_SIZE);
    }
    else if (buffer_size == MAX_SIZE) {
        mvprintw(40, 100, "buffer size is full: %ld/%d", buffer_size, MAX_SIZE);
        return -1;
    }

    //buf[buffer_size++] = ch;

    buf[s_global_info->new_size++] = ch;
    return buffer_size;
}

static int
pop(int *buf)
{
    if (s_global_info->new_size == 0) {
        mvprintw(40, 100, "buffer is empty: %ld/%d", buffer_size, MAX_SIZE);
        return -1;
    }
    //int chr = buf[--buffer_size];
    int chr = buf[--s_global_info->new_size];
    return chr;
}
