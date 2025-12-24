CC = gcc
OBJS = main.c std/iobuf.c std/std.c std/outstr.c
FLAGS = -Wall -Wextra

bta: $(OBJS)
	$(CC) $(FLAGS) $^ -o $@
