CC=gcc
CFLAGS=-Wall -Wextra -Werror
NAME=philo

FILES=main.c ft_atoi.c fred.c friends.c forks.c utils_phil.c utils_fork1.c utils_fork2.c get.c
OBJECTS=$(FILES:.c=.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -g -o $(NAME) -lpthread

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
