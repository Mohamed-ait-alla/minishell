/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:07:37 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/10 20:27:46 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_here_doc_fd(t_commands *cmds)
{
	int	i;

	i = 0;
	while (cmds->fds_of_heredoc[i] != -1)
		i++;
	return (cmds->fds_of_heredoc[--i]);
}

void	check_for_redirections(t_commands *cmds, char *tmpfile, int is_builtin, int *has_return)
{
	int	i;
	int	here_doc_fd;

	i = 0;
	while (cmds->input_file && cmds->input_file[i])
	{
		if (cmds->heredoc)
		{
			here_doc_fd = get_here_doc_fd(cmds);
			redirect_input_to_file_here_doc(here_doc_fd);
		}
		else
			redirect_input_to_file(cmds, cmds->input_file[i], is_builtin, &g_exit_status, has_return);
		i++;
	}
	i = 0;
	while (cmds->output_file && cmds->output_file[i])
	{
		if (cmds->append)
			redirect_output_to_file(cmds, cmds->output_file[i], 'a', is_builtin, &g_exit_status, has_return);
		else
			redirect_output_to_file(cmds, cmds->output_file[i], 'o', is_builtin, &g_exit_status, has_return);
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
	int		has_return;
	
	status = 0;
	n_of_cmds = count_n_of_cmds(cmds);
	tmpfile = NULL;
	if (n_of_cmds > 1)
	{
		handle_pipes(cmds, tmpfile, n_of_cmds, exec_env->env);
		printf("exit status in pipes is %d\n", g_exit_status);	
	}
	else
	{
		// check for builtins
		if (cmds->args && is_builtin(cmds->args[0]))
		{
			saved_stdout = dup(STDOUT_FILENO);
			saved_stdin = dup(STDIN_FILENO);
			has_return = false;
			check_for_redirections(cmds, tmpfile, true, &has_return);
			if (has_return)
				return (0);		
			status = execute_builtin(cmds->args, exec_env, g_exit_status);
			g_exit_status = status;
			if (ft_strncmp(cmds->args[0], "exit", ft_strlen("exit")) == 0)
			{
				printf("exit\n");
				ft_malloc(0, 0);
				exit (g_exit_status);
			}
			dup2 (saved_stdout, STDOUT_FILENO);
			dup2 (saved_stdin, STDIN_FILENO);
			close (saved_stdout);
			close (saved_stdin);
		}
		// single external command
		else
		{
			pid = fork();
			if (pid == -1)
				perror("fork: ");
			if (pid == 0)
			{
				handle_child_signals();
				check_for_redirections(cmds, tmpfile, false, false);
				execute_command(cmds, cmds->args, exec_env->env);
			}
			signal (SIGINT, SIG_IGN);
			waitpid(pid, &status, 0);
			handle_parent_signals();
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status)) 
				g_exit_status = 128 + WTERMSIG(status); // 128 + which sig has occured SIGQUIT = 3, SIGINT = 2
			if (g_exit_status == 130)
				printf("\n");
			if (g_exit_status == 131)
				printf("Quit (core dumped)\n"); 
		}
	}
}
