/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:15:03 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/19 19:42:26 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// paring the command
void	parsing_cmd(char *input, char **env)
{
	int			i;
	t_token		*tokens;
	t_env		*env_list;
	t_commands	*cmd_list;
	t_token		*tmp_token;
	t_env		*tmp_env;
	int			x;

	i = 0;
	// handle the exit cmd
	if ((input == NULL || !ft_strncmp(input, "exit", 4))
		&& (ft_strlen(input) == 4))
	{
		printf("exit\n");
		while (i != 99999999)
			i++;
		free(input);
		exit(0);
	}
	// split the cmd to tokens
	tokens = tokenize_input(input);
	// print tokens => value & type
	tmp_token = tokens;
	while (tmp_token)
	{
		printf("TOKEN: [%s] Type: %d\n", tmp_token->value, tmp_token->type);
		tmp_token = tmp_token->next;
	}
	// store the env variables in the env list
	env_list = init_env(env);
	// print env list
	tmp_env = env_list;
	while (tmp_env)
	{
		if (tmp_env->value)
			printf("%s=%s\n", tmp_env->key, tmp_env->value);
		else
			printf("Env: %s\n", tmp_env->key);
		tmp_env = tmp_env->next;
	}
	// parse the tokens
	cmd_list = parse_tokens(tokens);
	// print commands
	x = 0;
	while (cmd_list)
	{
		printf("Command %d:\n", x++);
		if (cmd_list->args)
		{
			printf("  Args:\n");
			for (int j = 0; cmd_list->args[j]; j++)
				printf("    %s\n", cmd_list->args[j]);
		}
		if (cmd_list->input_file)
			printf("  Input file: %s\n", cmd_list->input_file);
		if (cmd_list->output_file)
			printf("  Output file: %s (%s)\n", cmd_list->output_file,
				cmd_list->append ? "append" : "overwrite");
		cmd_list = cmd_list->next;
	}
	// ---------- Execution Part ----------
	// free token list and command list after execution
	free_tokens(tokens);
	free_commands(cmd_list);
}

int	main(int ac, char **av, char **env)
{
	char	*input;

	(void)av;
	// check if we have any args
	if (ac != 1)
		custom_error("Error: run only the programme", "", 1);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (ft_strlen(input) > 0)
			add_history(input);
		// paring the command
		parsing_cmd(input, env);
		free(input);
	}
}
