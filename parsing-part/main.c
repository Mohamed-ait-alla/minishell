/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:15:03 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/12 17:48:37 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_exit_status; // global var for exit status

static int	is_only_spaces(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] > 32)
			return (0);
		i++;
	}
	return (1);
}

// paring the command
void	parsing_cmd(char *input, t_exec_env *exec_env)
{
	int			i;
	t_token		*tokens;
	t_env		*env_list;
	t_commands	*cmd_list;
	t_commands	*tmp_cmd_list;
	t_token		*tmp_token;
	t_env		*tmp_env;
	int			x;
	char		*line_of_heredoc;
	int			j;
	char		*result;
	char		*tmp;
	int			start;
	char		*key;
	char		*value;
	char		*file_name;
	int			fd;
	int			idx_last_fd;
	char		*tmp_line;

	// char		*value_of_env;
	i = 0;
	// handle the exit cmd
	// if ((input == NULL || !ft_strcmp(input, "exit")) && (ft_strlen(input) == 4))
	// {
	// 	printf("exit\n");
	// 	free(input);
	// 	exit(0);
	// }
	// split the cmd to tokens
	tokens = tokenize_input(input);
	if (!tokens)
	{
		g_exit_status = 2;
		printf("minishell: syntax error\n");
		return ;
	}
	// printf("====================================================================>\n");
	// print tokens => value & type
	// tmp_token = tokens;
	// while (tmp_token)
	// {
	// 	printf("TOKEN: [%s] Type: %d Quote: %d\n", tmp_token->value, tmp_token->type, tmp_token->quote_type);
	// 	tmp_token = tmp_token->next;
	// }
	// store the env variables in the env list
	env_list = init_env(exec_env->env);
	// // print env list
	// tmp_env = env_list;
	// while (tmp_env)
	// {
	// 	if (tmp_env->value)
	// 		printf("%s=%s\n", tmp_env->key, tmp_env->value);
	// 	else
	// 		printf("Env: %s\n", tmp_env->key);
	// 	tmp_env = tmp_env->next;
	// }
	// // print value of env
	// value_of_env = get_env_value(env_list, "USER");
	// if (value_of_env)
	// 	printf("value of env: %s\n", value_of_env);
	// else
	// 	printf("env not found\n");
	// // expand the env variables
	expand_variables_and_remove_quotes(tokens, env_list);
	// print tokens after expanding the env variables
	// printf("Tokens after expanding:\n");
	// tmp_token = tokens;
	// while (tmp_token)
	// {
	// 	printf("TOKEN: [%s] Type: %d Quote type: %d\n", tmp_token->value, tmp_token->type, tmp_token->quote_type);
	// 	tmp_token = tmp_token->next;
	// }
	// parse the tokens
	cmd_list = parse_tokens(tokens);
	if (!cmd_list || !check_cmds(cmd_list))
	{
		g_exit_status = 2;
		printf("minishell: syntax error\n");
		return ;
	}
	
	// print commands	
	// x = 1;
	// tmp_cmd_list = cmd_list;
	// while (tmp_cmd_list)
	// {
	// 	printf("Command %d:\n", x++);
	// 	if (tmp_cmd_list->args)
	// 	{
	// 		printf("  Args:\n");
	// 		for (int j = 0; tmp_cmd_list->args[j]; j++)
	// 			printf("    %s\n", tmp_cmd_list->args[j]);
	// 		printf("Quote type: %d\n", tmp_cmd_list->quote_type);
	// 	}
	// 	if (tmp_cmd_list->input_file)
	// 	{
	// 		for (int i = 0; tmp_cmd_list->input_file[i]; i++)
	// 			printf("  Input file[%d]: %s\n", i + 1,
	// 				tmp_cmd_list->input_file[i]);
	// 	}
	// 	if (tmp_cmd_list->output_file)
	// 	{
	// 		for (int i = 0; tmp_cmd_list->output_file[i]; i++)
	// 		{
	// 			printf("  Output file[%d]: %s (%s) Type: %d\n", i + 1,
	// 				tmp_cmd_list->output_file[i],
	// 				tmp_cmd_list->append ? "append" : "overwrite", tmp_cmd_list->quote_type);
	// 		}
	// 	}
	// 	tmp_cmd_list = tmp_cmd_list->next;
	// }
	// create heredoc and store the fd in the cmd list
	if (heredoc(cmd_list, env_list) == -1)
	{
		printf("Error: heredoc failed\n");
		return ;
	}
	if (!cmd_list->args && cmd_list->heredoc)
		return ;
	// if (!cmd_list->args)
	// 	return;
	// if (cmd_list->args)
	// {
	// 	printf("yesfjk\n");
	// 	printf("cmd args is %s\n", cmd_list->args[0]);
	// }
	// idx_last_fd = 0;
	// while (cmd_list->fds_of_heredoc[idx_last_fd] != -1)
	// 	idx_last_fd++;
	// idx_last_fd--;
	// int z = 0;
	// while ((tmp_line = get_next_line(cmd_list->fds_of_heredoc[idx_last_fd])))
	// {
	// 	if (!tmp_line)
	// 		break ;
	// 	printf("%d: %s", z, tmp_line);
	// 	free(tmp_line);
	// 	z++;
	// }
	// ---------- Execution Part ----------
	tested_main_with_parsing(cmd_list, exec_env);
	// free token list and command list after execution
	// free_tokens(tokens);
	// free_commands(cmd_list);
	// ft_malloc(0, 0);
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_exec_env	envir;

	envir.env = copy_env(envp);
	(void)av;
	// check if we have any args
	if (ac != 1)
		custom_error("Error: run only the programme", "", EXIT_FAILURE, false);
	// update shell level
	update_shell_level(&envir);
	// handle the signal SIGQUIT (Ctrl-\)
	// handle_parent_signals();
	handle_parent_signals();
	while (1)
	{
		// input = readline("minishell> ");
		// input = readline("\033[1;92m➜  \033[1;36mminishell> \033[0m ");
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strlen(input) > 0)
			add_history(input);
		// paring the command
		if (is_only_spaces(input))
		{
			free(input);
			continue ;
		}
		parsing_cmd(input, &envir);
		free(input);
	}
	ft_malloc(0, 0);
	// clear history
	clear_history();
}
