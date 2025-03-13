CC = cc
CFLAGS = #-Wall -Wextra -Werror
RM = rm -f

NAME = minishell

SRCS = main.c

S_OBJS = $(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(S_OBJS)
	$(CC) $(CFLAGS) $(S_OBJS) -o ${NAME}

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(S_OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: ${S_OBJS}
