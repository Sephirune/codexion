SRC_DIR = src

SRCS			=	$(SRC_DIR)/coders/parser.c\


OBJS			=	$(SRCS:.c=.o)

CC			=	cc
RM			=	rm -f
CFLAGS			=	-Wall -Wextra -Werror -I -pthread

NAME			=	codexion

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all:			$(NAME)

$(NAME):	$(OBJS) $(PUSH_SWAP_OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS) $(PUSH_SWAP_OBJS) $(CHECKER_OBJS)

fclean:	clean
	$(RM) $(NAME) $(BONUS_NAME)

re:			fclean 
			$(NAME) all

.PHONY:			all clean fclean re