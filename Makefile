NAME = minishell

SRCS = main.c \
       ./parsing-part/custom_error.c \
       ./parsing-part/is_only_spaces.c \
       ./parsing-part/tokenizer.c \
       ./parsing-part/get_token_type.c \
       ./parsing-part/process_input.c \
       ./parsing-part/parse_tokens.c \
       ./parsing-part/check_cmds.c \
	   ./parsing-part/ft_realloc_array.c \
	   ./parsing-part/init_env.c \
	   ./parsing-part/get_env_value.c \
	   ./parsing-part/expand_variables_and_remove_quotes.c \
	   ./parsing-part/cases_of_expanding.c \
	   ./parsing-part/cases_of_expanding_plus.c \
	   ./parsing-part/ft_strjoin_char.c \
	   ./parsing-part/heredoc.c \
	   ./parsing-part/heredoc_utils.c \
	   ./parsing-part/ft_malloc.c \
	   ./parsing-part/print_error.c \
	   ./execution-part/executor.c \
	   ./execution-part/pipes_execution.c \
	   ./execution-part/pipes_utils.c \
	   ./execution-part/redirect_io.c \
	   ./execution-part/redirect_io_utils.c \
	   ./execution-part/run_builtin_commands.c \
	   ./execution-part/run_external_commands.c \
	   ./execution-part/builtins/builtin_cd.c \
	   ./execution-part/builtins/builtin_exit.c \
	   ./execution-part/builtins/builtin_export.c \
	   ./execution-part/builtins/export_utils_1.c \
	   ./execution-part/builtins/export_utils_2.c \
	   ./execution-part/builtins/builtin_env.c \
	   ./execution-part/builtins/builtin_unset.c \
	   ./execution-part/builtins/builtin_echo.c \
	   ./execution-part/builtins/builtin_pwd.c \
	   ./execution-part/utils.c \
	   ./execution-part/signals.c \
	   ./execution-part/here_doc_signals.c \
	   

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
RM = rm -f

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -lreadline -o $(NAME) $(OBJS) $(LIBFT)

$(LIBFT):
	make bonus -C $(LIBFT_DIR)

all: $(NAME)

clean:
	$(RM) $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

.SECONDARY: ${OBJS}
