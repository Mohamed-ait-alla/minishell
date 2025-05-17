/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:40:24 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/16 20:27:43 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_unused_pipes(int (*pipes)[2], int n_of_pipes, int except)
{
	int	i;

	i = 0;
	while (i < n_of_pipes)
	{
		if (i != except)
		{
			close (pipes[i][0]);
			close (pipes[i][1]);
		}
		i++;
	}
}

void	wait_for_childs(t_commands *cmds, int *pids, int n_of_cmds)
{
	int	status;
	int	i;

	status = 0;
	i = 0;
	while (i < n_of_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status) && (i == n_of_cmds - 1))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
		i++;
	}
}

void	allocate_pipes_and_pids(int (**pipes)[2], int **pids, int n_of_cmds)
{
	*pipes = ft_malloc(sizeof(int [2]) * (n_of_cmds - 1), 1);
	*pids = ft_malloc(sizeof(int) * n_of_cmds, 1);
	if (!pids || !pipes)
	{
		perror("Failed to allocate pipes or pids: ");
		return ;
	}
}

void	create_pipes(int (*pipes)[2], int n_of_cmds)
{
	int	i;

	i = 0;
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
}
