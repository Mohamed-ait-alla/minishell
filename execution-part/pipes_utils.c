/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:40:24 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/20 21:50:18 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_input_redirections(t_redirections *redirections, t_commands *cmds)
{
	t_redirections	*current;
	int				redirected;

	redirected = 0;
	if (!redirections)
		return (0);
	current = redirections;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_IN)
		{
			redirect_input_to_file(cmds, 0, &g_exit_status, NULL);
			redirected = 1;
		}
		else if (current->type == TOKEN_HEREDOC)
		{
			if (!cmds->args && cmds->heredoc)
				return (0);
			redirect_input_to_file_here_doc(cmds->here_doc_file);
			return (1);
		}
		current = current->next;
	}
	return (redirected);
}

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

void	wait_for_childs(int *pids, int n_of_cmds)
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
		if (g_exit_status == 130)
			printf("\n");
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
