main_file = src/main.c
file_load = src/load_file.c

CC = gcc
out = main

all:
	$(CC) $(main_file) $(file_load) -o $(out)
