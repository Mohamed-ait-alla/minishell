/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:41:13 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/15 13:40:52 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

static void	configure_pipeline_io(t_commands *cmds, int pipes[][2], int i, int n_of_cmds, char *tmpfile, int is_builtin, int *has_return)
{
	if (i == 0) // first command 
	{
		check_for_redirections(cmds, tmpfile, is_builtin, has_return);
		if (*has_return == 1 || *has_return == -1 || (*has_return == -2 && is_builtin))
		{
			if (*has_return == -1)
				*has_return = false;
			return ;
		}
		redirect_output_to_pipe(pipes[i][1]);
	}
	else if (i == n_of_cmds - 1) // last command
	{
		check_for_redirections(cmds, tmpfile, is_builtin, has_return); // take care when here_doc is found
		if (*has_return || *has_return == -1 || *has_return == -2)
		{
			if (*has_return == -1)
				*has_return = false;
			return ;
		}
		redirect_input_to_pipe(pipes[i - 1][0]);
	}
	else // middle commands
	{
		check_for_redirections(cmds, tmpfile, is_builtin, has_return);
		if (*has_return || *has_return == -1 || *has_return == -2)
		{
			if (*has_return == -1)
				*has_return = false;
			return ;
		}
		redirect_input_to_pipe(pipes[i - 1][0]);
		redirect_output_to_pipe(pipes[i][1]);
	}
}

static void	wait_for_childs(t_commands *cmds, int pids[], int n_of_cmds, char *tmpfile)
{
	int	status;
	int	i;

	status = 0;
	i = 0;
	while (i < n_of_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status) && (i == n_of_cmds - 1))
		{
			g_exit_status = WEXITSTATUS(status);
		}
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
		i++;
	}
}

static void	execute_pipes(t_commands *cmds, int n_of_cmds, char *tmpfile, t_exec_env *exec_env)
{
	t_commands *tmp;
	int pids[n_of_cmds];
	int pipes[n_of_cmds - 1][2];
	int i;
	int has_return;

	i = 0;
	has_return = 0;
	tmp = cmds;
	// creates the pipe ends
	while (i < n_of_cmds - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("an error occured while creating pipes: ");
			g_exit_status = EXIT_FAILURE;
			return ;
		}
		i++;
	}
	i = 0;
	while (tmp && i < n_of_cmds)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("an error occured while forking processes: ");
			g_exit_status = EXIT_FAILURE;
			return ;
		}
		if (pids[i] == 0) // child processes
		{
			// check if it is builtin
			if (tmp->args && is_builtin(tmp->args[0]))
			{
				configure_pipeline_io(tmp, pipes, i, n_of_cmds, NULL, true, &has_return);
				close_unused_pipes(pipes, n_of_cmds - 1, -1);
				if (has_return)
					exit (g_exit_status);
				execute_builtin(tmp->args, exec_env, 0);
				exit (g_exit_status);
			}
			else
			{
				// printf("yes external");
				configure_pipeline_io(tmp, pipes, i, n_of_cmds, NULL, false, &has_return);
				close_unused_pipes(pipes, n_of_cmds - 1, -1);
				execute_command(tmp, tmp->args, exec_env->env);
			}
		}
		i++;
		tmp = tmp -> next;
	}
	close_unused_pipes(pipes, n_of_cmds - 1, -1);
	wait_for_childs(cmds, pids, n_of_cmds, tmpfile);
}

void	handle_pipes(t_commands *cmds, char *tmpfile, int n_of_cmds, t_exec_env *exec_env)
{
	execute_pipes(cmds, n_of_cmds, tmpfile, exec_env);
}