CC=gcc
FLAGS=-m32
DEBUG_FLAGS=-g -fsanitize=address
SRC=fork.c segfault.c
BINS=$(SRC:%.c=%) $(SRC:%.c=%_32)
LIBS=
OBJ=
RM =rm -rf

all:
	$(CC) $(FLAGS) fork.c -o fork
	$(CC) $(FLAGS) fork.c -o fork_32
	$(CC) $(FLAGS) segfault.c -o segfault
	$(CC) $(FLAGS) segfault.c -o segfault_32

clean:

fclean: clean
	$(RM) $(BINS)

re: fclean all

