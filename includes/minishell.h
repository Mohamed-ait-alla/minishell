/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:42:01 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/18 11:40:43 by mait-all         ###   ########.fr       */
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
# include <readline/history.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
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

typedef struct s_exec_env
{
	int						is_created;
	char					**env;
}							t_exec_env;

typedef struct s_exec_pipe
{
	int						index;
	int						n_of_cmds;
	int						is_builtin;
	int						has_return;
	int						*pids;
	int						(*pipes)[2];
}							t_exec_pipe;

int							custom_error(char *err_msg, char *arg,
								int exit_code, int is_builtin);
int							is_only_spaces(char *input);
t_token						*tokenize_input(char *input);
char						*get_operator(char *input, int *i,
								t_quote_type *quote_type);
char						*get_word(char *input, int *i,
								t_quote_type *quote_type);
int							ft_strncmp(const char *s1, const char *s2,
								size_t n);
t_token_type				get_token_type(char *value);
char						*process_input(char *input, int *i,
								t_quote_type *quote_type);
t_commands					*parse_tokens(t_token *tokens);
int							check_cmds(t_token *tokens);
char						**ft_realloc_array(char **arr, char *new_str);
t_env						*init_env(char **env);
char						*get_env_value(t_env *env, char *key);
void						expand_variables_and_remove_quotes(t_token *tokens,
								t_env *env);
char						*ft_strjoin_char(char *str, char c);
char						*case_of_squote(char *word, int *i, char *result);
char						*case_of_dquote(char *word, int *i, char *result,
								t_env *env);
char						*case_of_normal_var(char *word, int *i,
								char *result, t_env *env);
char						*case_of_var_with_next_char_squote(char *word,
								int *i, char *result);
char						*case_of_var_with_next_char_dquote(char *word,
								int *i, char *result);
char						*case_of_var_with_next_char_digit(char *word,
								int *i, char *result);
char						*case_of_var_with_exit_status(int *i, char *result);
char						*case_of_word(char *word, int *i, char *result);
int							heredoc(t_commands *cmds, t_env *env);
int							count_here_doc(t_commands *cmds, char ***files);
int							count_redirections(t_commands *cmds);
char						*expand_the_heredoc(char *input_heredoc,
								t_env *env);
void						ignore_ctrl_c_with_exit_status(int pid,
								int *status);
void						unlink_files(int total_here_doc, char **files);
void						*ft_malloc(size_t size, int mode);

int							launch_execution(t_commands *cmds,
								t_exec_env *exec_env);

int							redirect_output_to_file(t_commands *cmds,
								int is_builtin, int *exit_status,
								int *has_return);
void						redirect_output_to_pipe(int write_pipe_end);
int							redirect_input_to_file(t_commands *cmds,
								int is_builtin, int *exit_status,
								int *has_return);
void						redirect_input_to_file_here_doc(char *heredoc_file);
char						*get_tmp_file(void);
void						redirect_input_to_pipe(int read_pipe_end);
int							check_for_redirections(t_commands *cmds,
								int is_builtin, int *has_return);
bool						has_space(char *str);
int							handle_ambigous_redirect(int is_builtin,
								int *has_return, int *exit_status);
int							set_output_flags(t_commands *cmd);
int							handle_open_errors(t_commands *cmds, int is_builtin,
								int *has_return, int *exit_status);

void						handle_pipes(t_commands *cmds, int n_of_cmds,
								t_exec_env *exec_env);
void						close_unused_pipes(int (*pipes)[2], int n_of_pipes,
								int except);
int							handle_input_redirections(
								t_redirections *redirections,
								t_commands *cmds);
void						wait_for_childs(int *pids, int n_of_cmds);
void						allocate_pipes_and_pids(int (**pipes)[2],
								int **pids, int n_of_cmds);
void						create_pipes(int (*pipes)[2], int n_of_cmds);

void						execute_command(char **args,
								char **env);

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

void						manage_shell_errors(char *arg, char *path);

char						**copy_env(char **envp);
void						update_shell_level(t_exec_env *exec_env);

void						handle_parent_signals(void);
void						handle_child_signals(void);
void						handle_here_doc_signals(void);

int							builtin_cd(char **args, char **env, int is_created);
int							builtin_echo(char **args);
int							builtin_env(char **env);
int							builtin_exit(char	**args,
								int last_cmd_exit_status);
int							builtin_export(char **args, t_exec_env *exec_env);
int							builtin_pwd(char **env);
int							builtin_unset(char **args, char **env,
								int is_created);
void						add_var_to_env(t_exec_env *exec_env, char *var);
void						append_env_var(t_exec_env *exec_env,
								char *new_value, int is_found);
void						sort_env(char **env);
void						print_sorted_env(t_exec_env *exec_env);
bool						is_valid_identifier(char *arg);
int							ft_get_env_len(char **env);
int							ft_get_env_var_len(char *env_var);
int							ft_max(int value1, int value2);
void						ft_swap(char **s1, char **s2);

#endif