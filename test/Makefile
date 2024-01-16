CC=gcc
FLAGS=
DEBUG_FLAGS=-g -fsanitize=address
SRC=$(wildcard *.c)
BINS=$(SRC:%.c=%)
LIBS=
OBJ=
RM =rm -rf

all: $(BINS)

%: $(SRC)/%.c
	$(CC) $(CFLAGS) -o $(BIN)/$@ $<

clean:

fclean: clean
	$(RM) $(BINS)

re: fclean all

run: re
	./$(NAME)

debug: fclean
	$(CC) $(FLAGS) $(DEBUG_FLAGS) $(LIBS) $(SRC) -o $(NAME)
