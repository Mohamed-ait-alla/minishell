/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:07:37 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/07 13:52:36 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_for_redirections(t_commands *cmds, char *tmpfile)
{
	int	i;

	i = 0;
	while (cmds->input_file && cmds->input_file[i]) 
	{
		if (cmds->heredoc)
				redirect_input_to_file_here_doc(cmds, cmds->input_file[i], tmpfile);
		else
				redirect_input_to_file(cmds->input_file[i]);
		i++;
	}
	i = 0;
	while (cmds->output_file && cmds->output_file[i])
	{
		if (cmds->append)
			redirect_output_to_file(cmds->output_file[i], 'a');
		else
			redirect_output_to_file(cmds->output_file[i], 'o');
		i++;
	}
}

int	count_n_of_cmds(t_commands *cmds)
{
	int count;
	t_commands *tmp;

	count = 0;
	tmp = cmds;
	while (tmp)
	{
		count ++;
		tmp = tmp->next;
	}
	return (count);
}

int	tested_main_with_parsing(t_commands *cmds, t_exec_env *exec_env)
{
	char	*tmpfile;
	int		pid;
	int		status;
	int		n_of_cmds;
	int		saved_stdin;
	int		saved_stdout;
	
	status = 0;
	n_of_cmds = count_n_of_cmds(cmds);
	tmpfile = NULL;
	if (n_of_cmds > 1)
	{
		handle_pipes(cmds, tmpfile, n_of_cmds, exec_env->env);
		printf("exit status in pipes is %d\n", cmds->exit_status);	
	}
	else
	{
		// check for builtins
		if (cmds->args && is_builtin(cmds->args[0]))
		{
			saved_stdout = dup(STDOUT_FILENO);
			saved_stdin = dup(STDIN_FILENO);
			check_for_redirections(cmds, tmpfile);
			status = execute_builtin(cmds->args, exec_env, cmds->exit_status);
			cmds->exit_status = status;
			printf("exit status in builtins is %d\n", cmds->exit_status);
			if (ft_strncmp(cmds->args[0], "exit", ft_strlen("exit")) == 0)
				exit (cmds->exit_status);
			dup2 (saved_stdout, STDOUT_FILENO);
			dup2 (saved_stdin, STDIN_FILENO);
			close (saved_stdout);
			close (saved_stdin);
		}
		// single external command
		else
		{
			handle_child_signals();
			pid = fork();
			if (pid == -1)
				perror("fork: ");
			if (pid == 0)
			{
				check_for_redirections(cmds, tmpfile);
				execute_command(cmds, cmds->args, exec_env->env);
			}
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				cmds->exit_status = WEXITSTATUS(status);
			printf("exit status is %d\n", cmds->exit_status);
		}
	}
}
