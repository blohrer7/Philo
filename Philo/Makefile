NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./include

SRC =	src/main.c \
		src/init.c \
		src/utils.c \
		src/philos.c \
		src/monitor.c \
		src/eat_forks.c \


OBJ = $(SRC:.c=.o)

all: $(NAME)
		@echo "\n\033[1;33m Use for example: ./philo 5 800 200 200 7 \033[0m\n"

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lpthread

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
