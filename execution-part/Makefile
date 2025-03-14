CC = cc
CFLAGS = #-Wall -Wextra -Werror
RM = rm -f

NAME = minishell

SRCS = main.c

S_OBJS = $(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(S_OBJS) | LIB
	$(CC) $(CFLAGS) $(S_OBJS) -L../libft -lft -o ${NAME}

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

LIB: 
	make -C ../libft

clean:
	$(RM) $(S_OBJS)
	make clean -C ../libft

fclean: clean
	$(RM) $(NAME)
	${RM} ../libft/libft.a

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: ${S_OBJS}
