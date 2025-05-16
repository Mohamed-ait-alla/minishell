/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:42:01 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/16 21:56:02 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define ERR_NO_FILE "minishell: no such file or directory: "
# define ERR_PERMISSION "minishell: permission denied: "
# define ERR_CMD_NOT_FOUND "minishell: command not found: "
# define ERR_AMBIG_REDIRECT "minishell: ambiguous redirect: "
# define PATH_MAX 1024
# define MAX_HEREDOCS 16
# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>

extern int					g_exit_status;

typedef enum s_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
}							t_token_type;

typedef enum s_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}							t_quote_type;

typedef struct s_redirections
{
	char					*file;
	t_token_type			type;
	struct s_redirections	*next;
}							t_redirections;

typedef struct s_token
{
	t_token_type			type;
	char					*value;
	t_quote_type			quote_type;
	struct s_token			*next;
}							t_token;

typedef struct s_commands
{
	char					**args;
	char					**env;
	// char					**input_file;
	// char					**output_file;
	int						exit_status;
	int						append;
	int						heredoc;
	char					*here_doc_file;
	t_quote_type			quote_type;
	t_redirections			*redirections;
	struct s_commands		*next;
}							t_commands;

typedef struct s_env
{
	char					*key;
	char					*value;
	struct s_env			*next;

}							t_env;

typedef struct s_gc
{
	void					*ptr;
	struct s_gc				*next;
}							t_gc;

// for testing purposes
typedef struct s_exec_env
{
	char					**env;
}							t_exec_env;

typedef struct s_exec_pipe
{
							int	index;
							int n_of_cmds;
							int	is_builtin;
							int	has_return;
							int	*pids;
							int	(*pipes)[2];
}							t_exec_pipe;


//       parsing-part function's declaration
int							custom_error(char *err_msg, char *arg,
								int exit_code, int is_builtin);
t_token						*tokenize_input(char *input);
int							ft_strncmp(const char *s1, const char *s2,
								size_t n);
t_commands					*parse_tokens(t_token *tokens);
int							check_cmds(t_token *tokens);
char						**ft_realloc_array(char **arr, char *new_str);
t_env						*init_env(char **env);
char						*get_env_value(t_env *env, char *key);
void						expand_variables_and_remove_quotes(t_token *tokens,
								t_env *env);
int							heredoc(t_commands *cmd, t_env *env);
void						*ft_malloc(size_t size, int mode);
char						*get_next_line(int fd);

//       execution-part function's declaration

//						#________ functions used for only testing purposes ________#
int							launch_execution(t_commands *cmds,
								t_exec_env *exec_env);

//						#________redirections________#
int						redirect_output_to_file(t_commands *cmds, int is_builtin,
								int *exit_status, int *has_return);
void						redirect_output_to_pipe(int write_pipe_end);
int						redirect_input_to_file(t_commands *cmds,
								int is_builtin, int *exit_status,
								int *has_return);
void						redirect_input_to_file_here_doc(char *here_doc_file);
char						*get_tmp_file(void);
void						redirect_input_to_pipe(int read_pipe_end);
int						check_for_redirections(t_commands *cmds, int is_builtin, int *has_return);

//						#________pipes________#
void						handle_pipes(t_commands *cmds,
								int n_of_cmds, t_exec_env *exec_env);
void						close_unused_pipes(int pipes[][2], int n_of_pipes,
								int except);

//						#________external commands________#
void						execute_command(t_commands *cmds, char **args,
								char **env);

//						#________built-in commands________#
int							is_builtin(char *cmd);
int							execute_builtin(char **args, t_exec_env *exec_env,
								int last_cmd_exit_status);
int							search_for_env_var(char **env, char *var);
int							has_equal_sign(char *var);
bool						is_valid_identifier(char *arg);
void						print_sorted_env(t_exec_env *exec_env);
void						sort_env(char **env);
void						ft_swap(char **s1, char **s2);
int							ft_max(int value1, int value2);
int							ft_get_env_var_len(char *env_var);
int							ft_get_env_len(char **env);

//						#________errors________#
void						manage_shell_errors(char *arg, char *path);

//						#________utils________#
char						**copy_env(char **envp);
void						update_shell_level(t_exec_env *exec_env);

//						#________signals________#
void						handle_parent_signals(void);
void						handle_child_signals(void);
void						handle_here_doc_signals(void);

#endif