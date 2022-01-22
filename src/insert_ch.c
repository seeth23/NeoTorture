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
insert(int32_t *buf, char ch, file_information *p)
{
    s_global_info = p;
    int res = push(buf, ch);
    //cursor_position = cur_pos;
    //addch((char)buf[s_global_info->cur_cursor.current.x]);
    //refresh();
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
    //mvprintw(40, 100, "deleted %c", ch);
}

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

    delete_on_pos(buf, s_global_info->cur_cursor.current.x);
    s_global_info->new_size--;
    return 0;
}

static void
delete_on_pos(int32_t *buffer, int32_t pos)
{
    for (size_t i = pos; i < s_global_info->new_size; i++) {
        buffer[i] = buffer[i + 1];
    }
}
