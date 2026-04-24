NAME		= codexion
CC		= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
RM		= rm -f

SRCS		= parser.c \
		  coder.c \
		  dongle_utils.c \
		  monitor.c \
		  sim_init.c \
		  utils.c \
		  main.c \
		  queue.c

OBJS		= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
