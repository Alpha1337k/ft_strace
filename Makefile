NAME=ft_strace

CC=gcc
FLAGS=-g -Wall -Werror -Wextra
DEBUG_FLAGS=-g -fsanitize=address
SRC=src/*.c
LIBS=
OBJ=
RM =rm -rf

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(FLAGS) $(LIBS) $(SRC) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	make -C test fclean
	$(RM) $(NAME)

tests:
	make -C test

re: fclean all

run: re
	./$(NAME)

debug: fclean
	$(CC) $(FLAGS) $(DEBUG_FLAGS) $(LIBS) $(SRC) -o $(NAME)
