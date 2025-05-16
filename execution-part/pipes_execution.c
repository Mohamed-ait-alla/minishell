/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:41:13 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/16 20:27:00 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	setup_redirections_and_broken_pipes(t_commands *cmds, t_exec_pipe *t_pipe)
{
	int	broken_condition;

	broken_condition = -1;
	check_for_redirections(cmds, t_pipe->is_builtin, &t_pipe->has_return);
	if (t_pipe->index == 0)
		broken_condition = t_pipe->has_return == 1 || t_pipe->has_return == -1
			|| (t_pipe->has_return == -2 && t_pipe->is_builtin);
	else
		broken_condition = t_pipe->has_return || t_pipe->has_return == -1
			|| t_pipe->has_return == -2;
	if (broken_condition)
	{
		if (t_pipe->has_return == -1)
			t_pipe->has_return = false;
		return (-1);
	}
	return (0);
}

static void	configure_pipeline_io(t_commands *cmds, int (*pipes)[2],
									t_exec_pipe *t_pipe)
{
	if (t_pipe->index == 0)
	{
		if (setup_redirections_and_broken_pipes(cmds, t_pipe) < 0)
			return ;
		redirect_output_to_pipe(pipes[t_pipe->index][1]);
	}
	else if (t_pipe->index == t_pipe->n_of_cmds - 1)
	{
		if (setup_redirections_and_broken_pipes(cmds, t_pipe) < 0)
			return ;
		redirect_input_to_pipe(pipes[t_pipe->index - 1][0]);
	}
	else
	{
		if (setup_redirections_and_broken_pipes(cmds, t_pipe) < 0)
			return ;
		redirect_input_to_pipe(pipes[t_pipe->index - 1][0]);
		redirect_output_to_pipe(pipes[t_pipe->index][1]);
	}
}

void	handle_child_proccesses(t_commands *cmd, int (*pipes)[2],
								t_exec_pipe *t_pipe, t_exec_env *exec_env)
{
	if (cmd->args && is_builtin(cmd->args[0]))
	{
		t_pipe->is_builtin = true;
		configure_pipeline_io(cmd, pipes, t_pipe);
		close_unused_pipes(pipes, t_pipe->n_of_cmds - 1, -1);
		if (t_pipe->has_return)
			exit (g_exit_status);
		execute_builtin(cmd->args, exec_env, 0);
		exit (g_exit_status);
	}
	else
	{
		t_pipe->is_builtin = false;
		configure_pipeline_io(cmd, pipes, t_pipe);
		close_unused_pipes(pipes, t_pipe->n_of_cmds - 1, -1);
		execute_command(cmd, cmd->args, exec_env->env);
	}
}

static void	execute_pipes(t_commands *cmds, int n_of_cmds,
							t_exec_env *exec_env, t_exec_pipe *t_pipe)
{
	t_commands	*tmp;
	int			i;

	tmp = cmds;
	i = 0;
	while (tmp && i < n_of_cmds)
	{
		t_pipe->pids[i] = fork();
		if (t_pipe->pids[i] == -1)
		{
			perror("an error occured while forking processes: ");
			g_exit_status = EXIT_FAILURE;
			return ;
		}
		if (t_pipe->pids[i] == 0)
		{
			t_pipe->index = i;
			handle_child_proccesses(tmp, t_pipe->pipes, t_pipe, exec_env);
		}
		i++;
		tmp = tmp -> next;
	}
	close_unused_pipes(t_pipe->pipes, n_of_cmds - 1, -1);
	wait_for_childs(cmds, t_pipe->pids, n_of_cmds);
}

void	handle_pipes(t_commands *cmds, int n_of_cmds, t_exec_env *exec_env)
{
	t_exec_pipe	t_pipe;

	ft_bzero(&t_pipe, sizeof(t_exec_pipe));
	t_pipe.n_of_cmds = n_of_cmds;
	allocate_pipes_and_pids(&t_pipe.pipes, &t_pipe.pids, n_of_cmds);
	create_pipes(t_pipe.pipes, n_of_cmds);
	execute_pipes(cmds, n_of_cmds, exec_env, &t_pipe);
}
