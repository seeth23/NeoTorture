main_file = src/main.c
file_load = src/load_file.c
hanlde_input = src/key_handle.c
insert_char = src/insert_ch.c
cursor = src/cursor.c

win_info = src/window_information.c


flags = -lncurses -Wall
CC = gcc
out = main

all:
	$(CC) $(main_file) $(file_load) $(hanlde_input) $(insert_char) $(win_info) $(cursor) $(flags) -o $(out)
