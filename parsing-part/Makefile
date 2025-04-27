NAME = minishell

SRCS = main.c \
       ../helper-functions/custom_error.c \
       ../helper-functions/tokenizer.c \
       ../helper-functions/parse_tokens.c \
	   ../helper-functions/ft_realloc_array.c \
	   ../helper-functions/free_tokens.c \
	   ../helper-functions/free_commands.c \
	   ../helper-functions/init_env.c \
	   ../helper-functions/get_env_value.c \
	   ../helper-functions/expand_varaiables.c \
	   ../execution-part/main.c \
	   ../execution-part/pipes_execution.c \
	   ../execution-part/pipes_utils.c \
	   ../execution-part/redirect_io.c \
	   ../execution-part/run_builtin_commands.c \
	   ../execution-part/run_external_commands.c \
	   ../execution-part/builtins/builtin_cd.c \
	   ../execution-part/builtins/builtin_exit.c \
	   ../execution-part/builtins/builtin_export.c \
	   ../execution-part/builtins/builtin_env.c \
	   ../execution-part/builtins/builtin_unset.c \
	   ../execution-part/builtins/builtin_echo.c \
	   ../execution-part/builtins/builtin_pwd.c \
	   ../execution-part/utils.c \
	   ../get_next_line/get_next_line.c \
	   ../get_next_line/get_next_line_utils.c
	   

LIBFT_DIR = ../libft
LIBFT = $(LIBFT_DIR)/libft.a

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = #-Wall -Wextra -Werror
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
