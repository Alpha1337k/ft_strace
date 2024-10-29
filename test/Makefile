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
	$(FLAGS) -o $@ $<

clean:

fclean: clean
	$(RM) $(BINS)

re: fclean all

