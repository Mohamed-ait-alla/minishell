SRCS = main.c \
       ../helper-functions/custom_error.c \
	   ../libft/ft_strncmp.c ../libft/ft_putstr_fd.c ../libft/ft_putchar_fd.c ../libft/ft_strlen.c

OBJS = $(SRCS:.c=.o)
NAME = minishell
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -lreadline -o $(NAME) $(OBJS)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.SECONDARY: ${OBJS}

.PHONY: all clean fclean re