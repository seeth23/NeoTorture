main_file = src/main.c
file_handler = src/file_handler.c
hanlde_input = src/key_handle.c
insert_char = src/insert_ch.c
cursor = src/cursor.c
current_time = src/current_time.c
win_info = src/window_information.c


flags = -lncurses -Wall
CC = gcc
out = main

all:
	$(CC) $(main_file) $(file_handler) $(hanlde_input) $(insert_char) $(win_info) $(cursor) $(current_time) $(flags) -o $(out)
