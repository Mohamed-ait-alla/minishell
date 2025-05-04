CC = cc
CFLAGS = #-Wall -Wextra -Werror
RM = rm -f

NAME = minishell

SRCS = main.c \
	   redirect_io.c \
	   pipes_execution.c \
	   pipes_utils.c \
	   run_external_commands.c \
	   run_builtin_commands.c \
	   builtins/builtin_cd.c \
	   builtins/builtin_echo.c \
	   builtins/builtin_env.c \
	   builtins/builtin_exit.c \
	   builtins/builtin_export.c \
	   builtins/builtin_pwd.c \
	   builtins/builtin_unset.c \
	   utils.c \
	   ../helper-functions/custom_error.c \
	   ../get_next_line/get_next_line.c \
	   ../get_next_line/get_next_line_utils.c

S_OBJS = $(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(S_OBJS) | LIB
	$(CC) $(CFLAGS) $(S_OBJS) -L../libft -lft -lreadline -o ${NAME}

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
