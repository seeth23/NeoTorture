#include "ncurses.h"
#include "stdio.h"
#include "stdio.h"
#include "string.h"

#include "file_handler.h"
#include "file_info.h"

#define LIST_SIZE 512

struct new_line_list*
init_line_list()
{
    struct new_line_list *tmp = malloc(sizeof(struct new_line_list));
    memset(tmp, 0, sizeof(struct new_line_list));
    if (!tmp) {
        endwin();
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    tmp->next = NULL;
    return tmp;
}

void
update_line_list(struct new_line_list *list_ptr, file_information *info_ptr)
{
    if (list_ptr->next == NULL) {
        list_ptr->line.column = info_ptr->cur_cursor.current.x;
        list_ptr->line.row = info_ptr->cur_cursor.current.y;
        list_ptr->next = init_line_list();
    } else {
        struct new_line_list *head = list_ptr;
        while (head->next != NULL) {
            head = head->next;
        }
        head->line.column = info_ptr->cur_cursor.current.x;
        head->line.row = info_ptr->cur_cursor.current.y;
        head->next = init_line_list();
    }
}

void
move_line_list(struct new_line_list *list_ptr, file_information *info_ptr)
{
    struct new_line_list *head = list_ptr;
    struct new_line_list *found = NULL;
    while (head->next != NULL) {
        if (head->line.row == info_ptr->cur_cursor.current.y) {
            found = head;
            found->line.column = info_ptr->cur_cursor.current.y;
            found->line.row = info_ptr->cur_cursor.current.x;
            found->next = head->next;
            //while (head->next->next != NULL) {
                //head->line.row;
            //}
            break;
        }
        head = head->next;
    }
}

int32_t
find_col(struct new_line_list *list_finder, file_information *info_ptr)
{
    struct new_line_list *head = list_finder;
    while (head->next != NULL) {
        if (head->line.row == info_ptr->cur_cursor.current.y) {
            return head->line.column;
        }
        head = head->next;
    }
    return -1;
}
