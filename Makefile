main_file = src/main.c
file_load = src/load_file.c
hanlde_input = src/key_handle.c



win_info = src/window_information.c


flags = -lncurses -Wall
CC = gcc
out = main

all:
	$(CC) $(main_file) $(file_load) $(hanlde_input) $(win_info) $(flags) -o $(out)
