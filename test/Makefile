CC=gcc
FLAGS=-m32
DEBUG_FLAGS=-g -fsanitize=address
SRC=$(wildcard *.c)
BINS=$(SRC:%.c=%)
LIBS=
OBJ=
RM =rm -rf

all: $(BINS)

%: $(SRC)/%.c
	$(CC) $(FLAGS) -o $(BIN)/$@ $<

clean:

fclean: clean
	$(RM) $(BINS)

re: fclean all

