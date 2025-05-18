/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:41:13 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/18 20:21:17 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_output_redirections(t_redirections *redirections,
										t_commands *cmds)
{
	t_redirections	*current;
	int				redirected;

	redirected = 0;
	if (!redirections)
		return (0);
	current = redirections;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_OUT)
		{
			cmds->redirections->file = current->file;
			redirect_output_to_file(cmds, 0, &g_exit_status, NULL);
			redirected = 1;
		}
		else if (current->type == TOKEN_APPEND)
		{
			redirect_output_to_file(cmds, 0, &g_exit_status, NULL);
			redirected = 1;
		}
		current = current->next;
	}
	return (redirected);
}

static void	configure_pipeline_io(t_commands *cmds, int (*pipes)[2],
									t_exec_pipe *t_pipe)
{
	int (redirected_in), (redirected_out);
	if (t_pipe->index == 0)
	{
		redirected_in = handle_input_redirections(cmds->redirections, cmds);
		redirected_out = handle_output_redirections(cmds->redirections, cmds);
		if (!redirected_out)
			redirect_output_to_pipe(pipes[t_pipe->index][1]);
	}
	else if (t_pipe->index == t_pipe->n_of_cmds - 1)
	{
		redirected_in = handle_input_redirections(cmds->redirections, cmds);
		if (!redirected_in)
			redirect_input_to_pipe(pipes[t_pipe->index - 1][0]);
		handle_output_redirections(cmds->redirections, cmds);
	}
	else
	{
		redirected_in = handle_input_redirections(cmds->redirections, cmds);
		if (!redirected_in)
			redirect_input_to_pipe(pipes[t_pipe->index - 1][0]);
		redirected_out = handle_output_redirections(cmds->redirections, cmds);
		if (!redirected_out)
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
			ft_exit(g_exit_status);
		execute_builtin(cmd->args, exec_env, 0);
		ft_exit(g_exit_status);
	}
	else
	{
		t_pipe->is_builtin = false;
		configure_pipeline_io(cmd, pipes, t_pipe);
		close_unused_pipes(pipes, t_pipe->n_of_cmds - 1, -1);
		if (t_pipe->index != t_pipe->n_of_cmds - 1 && cmd->heredoc && cmd->args)
			ft_exit(0);
		execute_command(cmd->args, exec_env->env);
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
	wait_for_childs(t_pipe->pids, n_of_cmds);
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
