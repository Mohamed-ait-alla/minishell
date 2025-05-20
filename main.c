/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:15:03 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/20 10:11:50 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int			g_exit_status;

static int	run_here_doc(t_commands *cmd_list, t_env *env_list)
{
	if (heredoc(cmd_list, env_list) == -1)
	{
		print_error("Error: heredoc failed\n");
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
		print_error("minishell: syntax error\n");
		return ;
	}





	

	// t_token	*tmp_token = tokens;
	// while (tmp_token)
	// {
	// 	printf("TOKEN: [%s] Type: %d Quote: %d\n", tmp_token->value, tmp_token->type, tmp_token->quote_type);
	// 	tmp_token = tmp_token->next;
	// }





	
	env_list = init_env(exec_env->env);
	expand_variables_and_remove_quotes(tokens, env_list);






	




	// printf("Tokens after expanding:\n");
	// tmp_token = tokens;
	// while (tmp_token)
	// {
	// 	printf("TOKEN: [%s] Type: %d Quote type: %d\n", tmp_token->value, tmp_token->type, tmp_token->quote_type);
	// 	tmp_token = tmp_token->next;
	// }









	
	cmd_list = parse_tokens(tokens);
	if (!cmd_list || !check_cmds(tokens))
	{
		g_exit_status = 2;
		print_error("minishell: syntax error\n");
		return ;
	}












	// int x = 1;
	// t_commands *tmp_cmd_list = cmd_list;
	// while (tmp_cmd_list)
	// {
	// 	printf("Command %d:\n", x++);
		
	// 	if (tmp_cmd_list->args)
	// 	{
	// 		printf("  Args:\n");
	// 		for (int j = 0; tmp_cmd_list->args[j]; j++)
	// 			printf("    %s\n", tmp_cmd_list->args[j]);
	// 		printf("  Quote type: %d\n", tmp_cmd_list->quote_type);
	// 	}
	
	// 	if (tmp_cmd_list->redirections)
	// 	{
	// 		t_redirections *redir = tmp_cmd_list->redirections;
	// 		int i = 1;
	// 		while (redir)
	// 		{
	// 			const char *type_redir;
	// 			if (redir->type == TOKEN_REDIRECT_IN)
	// 				type_redir = "Input";
	// 			else if (redir->type == TOKEN_REDIRECT_OUT)
	// 				type_redir = "Output (overwrite)";
	// 			else if (redir->type == TOKEN_APPEND)
	// 				type_redir = "Output (append)";
	// 			else if (redir->type == TOKEN_HEREDOC)
	// 				type_redir = "Heredoc";
	// 			else
	// 				type_redir = "Unknown";
	
	// 			printf("  Redirection[%d]: %s (%s)\n", i++, redir->file, type_redir);
	// 			redir = redir->next;
	// 		}
	// 	}
	// 	tmp_cmd_list = tmp_cmd_list->next;
	// }














	
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
			ft_malloc(0, 0);
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
	}
}

static char	**create_new_env(void)
{
	char	**new_env;
	char	*tmp;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	new_env = ft_malloc(sizeof(char *) * 4, 1);
	if (!new_env)
		return (NULL);
	tmp = ft_strjoin("PWD=", cwd);
	new_env[0] = ft_strdup(tmp);
	new_env[1] = ft_strdup("SHLVL=1");
	new_env[2] = ft_strdup("_=./minishell");
	new_env[3] = NULL;
	free(cwd);
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
