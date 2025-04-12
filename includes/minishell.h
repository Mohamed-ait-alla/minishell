/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:42:01 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/12 10:48:51 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define ERR_NO_FILE "bash: no such file or directory: "
# define ERR_PERMISSION "bash: permission denied: "
# define ERR_CMD_NOT_FOUND "bash: command not found: "
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <readline/readline.h>
# include "../libft/libft.h"
# include "../get_next_line/get_next_line.h"

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SEMICOLON,
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_commands
{
	char				**args;
	char				*input_file;
	char				*output_file;
	int					append;
	struct s_commands	*next;
}						t_commands;

//       parsing-part function's declaration
void	custom_error(char *err_msg, char *arg, int exit_code);
char	**split_the_cmd(char const *s);
int	ft_strncmp(const char *s1, const char *s2, size_t n);


//       execution-part function's declaration

//			#________redirections________#
void	redirect_output_to_file(char *file, char mode);
void	redirect_output_to_pipe(int write_pipe_end);
void	redirect_input_to_file(char *file);
void	redirect_input_to_file_here_doc(char *limitter, char *tmpfile);
char	*get_tmp_file(void);
int		check_for_here_doc(char **av);
void	redirect_input_to_pipe(int read_pipe_end);
void	check_for_redirections(char **av, char *tmpfile);

//			#________pipes________#
bool	check_for_pipes(char **av);
void    handle_pipes(char **av, char **env, char *tmpfile);
void	close_unused_pipes(int pipes[][2], int n_of_pipes, int except);
int		calculate_number_of_pipes(char **av);
bool	check_for_pipes(char **av);

//			#________commands________#
void	execute_command(char *cmd, char **env);

//			#________errors________#
void	manage_shell_errors(char *arg, char *path);

#endif