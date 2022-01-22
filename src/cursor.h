#ifndef CURSOR_H
#define CURSOR_H

typedef struct {
    int y;
    int x;
} cursor_y_x;

typedef struct {
    cursor_y_x prev;
    cursor_y_x current;
} cursor;

#endif
