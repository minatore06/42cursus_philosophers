CC=gcc
CFLAGS=-Wall -Wextra -Werror
NAME=philo

FILES=main.c ft_atoi.c
OBJECTS=$(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
