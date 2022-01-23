#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "ncurses.h"
#include "string.h"

#include "file_info.h"

static file_information *s_global_info = NULL;

#define MAX_SIZE 4096

static int32_t push(int32_t *buf, int32_t ch);
static int32_t pop(int32_t *buf);

static void change_file_information(file_information *u)
{
    u->change_size = u->new_size - u->init_size;
    if (!u->changed)
        u->changed = true;
    s_global_info = u;
}

void
insert(int32_t *buf, int32_t ch, file_information *p)
{
    s_global_info = p;
    int32_t res = push(buf, ch);
    change_file_information(p);
    if (res == -1) {
        mvprintw(40, 100, "Failed to add %c to buffer", ch);
    }
}

void
delete(int32_t *buf, file_information *p)
{
    s_global_info = p;
    change_file_information(p);
    pop(buf);
}

static void insert_on_pos(int32_t *buffer);

static int32_t
push(int32_t *buf, int32_t ch)
{
    if (MAX_SIZE - s_global_info->new_size < 10) {
        mvprintw(40, 100, "buffer size is almost full: %ld/%d", s_global_info->new_size, MAX_SIZE);
    }
    else if (s_global_info->new_size == MAX_SIZE) {
        mvprintw(40, 100, "buffer size is full: %ld/%d", s_global_info->new_size, MAX_SIZE);
        return -1;
    }
    s_global_info->new_size++;
    buf += s_global_info->cur_cursor.current.x;
    insert_on_pos(buf);
    *buf = ch;
    return s_global_info->new_size;
}

static void delete_on_pos(int32_t *buffer, int32_t pos);

static int32_t
pop(int32_t *buf)
{
    if (s_global_info->new_size == 0) {
        mvprintw(40, 100, "buffer is empty: %ld/%d", s_global_info->new_size, MAX_SIZE);
        return -1;
    }
    s_global_info->new_size--;
    delete_on_pos(buf, s_global_info->cur_cursor.current.x);
    return 0;
}

static void
delete_on_pos(int32_t *buffer, int32_t pos)
{
    for (size_t i = pos + 1; i < s_global_info->new_size; i++) {
        buffer[i] = buffer[i + 1];
    }
}

static void
insert_on_pos(int32_t *buffer)
{
    size_t elements_after_cursor = s_global_info->new_size - s_global_info->cur_cursor.current.x - 1;
    size_t bytes_after_cursor = elements_after_cursor * sizeof(int32_t);
    memmove(buffer + 1, buffer, bytes_after_cursor);
}
