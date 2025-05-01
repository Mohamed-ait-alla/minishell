/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:15:03 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/01 10:54:06 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	// char		*value_of_env;
	i = 0;
	// handle the exit cmd
	if ((input == NULL || !ft_strcmp(input, "exit"))
		&& (ft_strlen(input) == 4))
	{
		printf("exit\n");
		free(input);
		exit(0);
	}
	// split the cmd to tokens
	tokens = tokenize_input(input);
	if (!tokens)
	{
		printf("syntax error\n");
		return ;
	}
	// printf("====================================================================>\n");
	// print tokens => value & type
	// tmp_token = tokens;
	// while (tmp_token)
	// {
	// 	printf("TOKEN: [%s] Type: %d\n", tmp_token->value, tmp_token->type);
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
	// 	printf("TOKEN: [%s] Type: %d\n", tmp_token->value, tmp_token->type);
	// 	tmp_token = tmp_token->next;
	// }
	// parse the tokens
	cmd_list = parse_tokens(tokens);
	// print commands
	
	x = 1;
	tmp_cmd_list = cmd_list;
	while (tmp_cmd_list)
	{
		printf("Command %d:\n", x++);
		if (tmp_cmd_list->args)
		{
			printf("  Args:\n");
			for (int j = 0; tmp_cmd_list->args[j]; j++)
				printf("    %s\n", tmp_cmd_list->args[j]);
		}
		if (tmp_cmd_list->input_file)
			printf("  Input file: %s\n", tmp_cmd_list->input_file);
		if (tmp_cmd_list->output_file)
			printf("  Output file: %s (%s)\n", tmp_cmd_list->output_file,
				tmp_cmd_list->append ? "append" : "overwrite");
		tmp_cmd_list = tmp_cmd_list->next;
	}
	// ---------- Execution Part ----------
	// tested_main_with_parsing(cmd_list, exec_env);
	// free token list and command list after execution
	free_tokens(tokens);
	free_commands(cmd_list);
}

void	sig_int_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	setup_signal(void)
{
	struct sigaction	sa_quit;

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
// any where you use execve, you need to fork the process
// if (pid == 0)
// {
// 	signal(SIGINT, SIG_DFL);
// 	signal(SIGQUIT, SIG_DFL);
// 	// execve(...)
// }


int	main(int ac, char **av, char **envp)
{
	char	*input;
	t_exec_env envir;

	envir.env = copy_env(envp);
	(void)av;
	// check if we have any args
	if (ac != 1)
		custom_error("Error: run only the programme", "", 1);
	// handle the signal SIGINT (Ctrl+C)
	setup_signal();
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
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
	// clear history
	clear_history();
}
