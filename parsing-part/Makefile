NAME = minishell

SRCS = main.c \
       ../helper-functions/custom_error.c \
       ../helper-functions/tokenizer.c \
       ../helper-functions/get_token_type.c \
       ../helper-functions/process_input.c \
       ../helper-functions/parse_tokens.c \
       ../helper-functions/check_cmds.c \
	   ../helper-functions/ft_realloc_array.c \
	   ../helper-functions/init_env.c \
	   ../helper-functions/get_env_value.c \
	   ../helper-functions/expand_variables_and_remove_quotes.c \
	   ../helper-functions/cases_of_expanding.c \
	   ../helper-functions/cases_of_expanding_plus.c \
	   ../helper-functions/ft_strjoin_char.c \
	   ../helper-functions/heredoc.c \
	   ../helper-functions/heredoc_utils.c \
	   ../helper-functions/ft_malloc.c \
	   ../execution-part/executor.c \
	   ../execution-part/pipes_execution.c \
	   ../execution-part/pipes_utils.c \
	   ../execution-part/redirect_io.c \
	   ../execution-part/redirect_io_utils.c \
	   ../execution-part/run_builtin_commands.c \
	   ../execution-part/run_external_commands.c \
	   ../execution-part/builtins/builtin_cd.c \
	   ../execution-part/builtins/builtin_exit.c \
	   ../execution-part/builtins/builtin_export.c \
	   ../execution-part/builtins/export_utils_1.c \
	   ../execution-part/builtins/export_utils_2.c \
	   ../execution-part/builtins/builtin_env.c \
	   ../execution-part/builtins/builtin_unset.c \
	   ../execution-part/builtins/builtin_echo.c \
	   ../execution-part/builtins/builtin_pwd.c \
	   ../execution-part/utils.c \
	   ../execution-part/signals.c \
	   ../execution-part/here_doc_signals.c \
	   

LIBFT_DIR = ../libft
LIBFT = $(LIBFT_DIR)/libft.a

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -g3 #-Wall -Wextra -Werror
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
