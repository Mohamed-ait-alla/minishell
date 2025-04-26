/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:42:01 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/26 11:48:18 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define ERR_NO_FILE "bash: no such file or directory: "
# define ERR_PERMISSION "bash: permission denied: "
# define ERR_CMD_NOT_FOUND "bash: command not found: "
# define PATH_MAX 1024
# include "../get_next_line/get_next_line.h"
# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

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

typedef enum s_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}						t_quote_type;

typedef struct s_token
{
	t_token_type		type;
	char				*value;
	t_quote_type		quote_type;
	struct s_token		*next;
}						t_token;

typedef struct s_commands
{
	char				**args;
	char				*input_file;
	char				*output_file;
	int					append;
	int					heredoc;
	struct s_commands	*next;
}						t_commands;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;

}						t_env;

//       parsing-part function's declaration
void					custom_error(char *err_msg, char *arg, int exit_code);
t_token					*tokenize_input(char *input);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
t_commands				*parse_tokens(t_token *tokens);
char					**ft_realloc_array(char **arr, char *new_str);
void					free_tokens(t_token *tokens);
void					free_commands(t_commands *cmds);
t_env					*init_env(char **env);
char					*get_env_value(t_env *env, char *key);
void					expand_variables(t_token *tokens, t_env *env);

//       execution-part function's declaration

# include "../execution-part/parsing-testing-file.h"
// warning: just for testing

//						#________ functions used for only testing purposes ________#
void					tested_main_with_parsing(t_commands *cmds, char **env);

//						#________redirections________#
void					redirect_output_to_file(char *file, char mode);
void					redirect_output_to_pipe(int write_pipe_end);
void					redirect_input_to_file(char *file);
void					redirect_input_to_file_here_doc(char *limitter,
							char *tmpfile);
char					*get_tmp_file(void);
int						check_for_here_doc(char **av);
void					redirect_input_to_pipe(int read_pipe_end);
void					check_for_redirections(t_command **cmds, char *tmpfile);

//						#________pipes________#
bool					check_for_pipes(char **av);
void					handle_pipes(t_command **cmds, char *tmpfile,
							int n_of_cmds);
void					close_unused_pipes(int pipes[][2], int n_of_pipes,
							int except);
// int		calculate_number_of_pipes(char **av);
// bool	check_for_pipes(char **av);

//						#________external commands________#
void					execute_command(char **args, char **env);

//						#________built-in commands________#
int						is_builtin(char *cmd);
int						execute_builtin(char **args, char **env);
int						search_for_env_var(char **env, char *var);
int						has_equal_sign(char *var);

//						#________errors________#
void					manage_shell_errors(char *arg, char *path);

//						#________utils________#
char					**copy_env(char **envp);
void					free_double_array(char **arr);

#endif