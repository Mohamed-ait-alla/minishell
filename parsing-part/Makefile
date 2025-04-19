NAME = minishell

SRCS = main.c \
       ../helper-functions/custom_error.c \
       ../helper-functions/tokenizer.c \
       ../helper-functions/parse_tokens.c \
	   ../helper-functions/ft_realloc_array.c \
	   ../helper-functions/free_tokens.c \
	   ../helper-functions/free_commands.c \

LIBFT_DIR = ../libft
LIBFT = $(LIBFT_DIR)/libft.a

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -lreadline -o $(NAME) $(OBJS) $(LIBFT)

$(LIBFT):
	make -C $(LIBFT_DIR)

all: $(NAME)

clean:
	$(RM) $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
