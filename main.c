/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:15:03 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/18 16:10:24 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int			g_exit_status;

static int	run_here_doc(t_commands *cmd_list, t_env *env_list)
{
	if (heredoc(cmd_list, env_list) == -1)
	{
		printf("Error: heredoc failed\n");
		return (0);
	}
	if (cmd_list->signal_detected)
		return (0);
	return (1);
}

static void	parsing_cmd(char *input, t_exec_env *exec_env)
{
	t_token		*tokens;
	t_env		*env_list;
	t_commands	*cmd_list;

	tokens = tokenize_input(input);
	if (!tokens)
	{
		g_exit_status = 2;
		printf("minishell: syntax error\n");
		return ;
	}
	env_list = init_env(exec_env->env);
	expand_variables_and_remove_quotes(tokens, env_list);
	cmd_list = parse_tokens(tokens);
	if (!cmd_list || !check_cmds(tokens))
	{
		g_exit_status = 2;
		printf("minishell: syntax error\n");
		return ;
	}
	if (!run_here_doc(cmd_list, env_list))
		return ;
	launch_execution(cmd_list, exec_env);
}

static void	read_line_process(t_exec_env *env)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strlen(input) > 0)
			add_history(input);
		if (is_only_spaces(input))
		{
			free(input);
			continue ;
		}
		parsing_cmd(input, env);
		free(input);
	}
}

static char	**create_new_env(void)
{
	char	**new_env;
	char	*tmp;

	new_env = malloc(sizeof(char *) * 4);
	if (!new_env)
		return (NULL);
	tmp = ft_strjoin("PWD=", getcwd(NULL, 0));
	new_env[0] = ft_strdup(tmp);
	new_env[1] = ft_strdup("SHLVL=1");
	new_env[2] = ft_strdup("_=./minishell");
	new_env[3] = NULL;
	return (new_env);
}

int	main(int ac, char **av, char **envp)
{
	t_exec_env	envir;

	if (!envp || !envp[0])
	{
		envir.env = create_new_env();
		envir.is_created = 1;
	}
	else
	{
		envir.env = copy_env(envp);
		envir.is_created = 0;
	}
	(void)av;
	if (ac != 1)
		custom_error("Error: run only the programme", "", EXIT_FAILURE, false);
	update_shell_level(&envir);
	handle_parent_signals();
	read_line_process(&envir);
	ft_malloc(0, 0);
	clear_history();
	return (0);
}
