/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:41:13 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/26 11:49:18 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

static void	configure_pipeline_io(t_commands *cmds, int pipes[][2], int i, int n_of_cmds, char *tmpfile)
{
	if (i == 0) // first command 
	{
		check_for_redirections(cmds, tmpfile);
		redirect_output_to_pipe(pipes[i][1]);
	}
	else if (i == n_of_cmds - 1) // last command
	{
		redirect_input_to_pipe(pipes[i - 1][0]);
		check_for_redirections(cmds, tmpfile); // take care when here_doc is found
	}
	else // middle commands
	{
		redirect_input_to_pipe(pipes[i - 1][0]);
		redirect_output_to_pipe(pipes[i][1]);
	}
}

static void	wait_for_childs(int pids[], int n_of_cmds, char *tmpfile)
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
			if (tmpfile) // when here doc is founded, remove the tmpfile
			{
				printf("tmpfile %s\n", tmpfile);
				if (unlink(tmpfile) == -1)
					perror("unlink: ");
			}
			exit(WEXITSTATUS(status));
		}
		i++;
	}
}

static void	execute_pipes(t_commands *cmds, int n_of_cmds, char *tmpfile, char **env)
{
	t_commands *tmp;
	int pids[n_of_cmds];
	int pipes[n_of_cmds - 1][2];
	int i;

	i = 0;
	tmp = cmds;
	// creates the pipe ends
	while (i < n_of_cmds - 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("an error occured while creating pipes: ");
			exit (EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	while (i < n_of_cmds)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("an error occured while forking processes: ");
			exit (EXIT_FAILURE);
		}
		if (pids[i] == 0) // child processes
		{
			configure_pipeline_io(tmp, pipes, i, n_of_cmds, tmpfile);
			close_unused_pipes(pipes, n_of_cmds - 1, -1);
			execute_command(tmp->args, env);
		}
		i++;
		tmp = tmp -> next;
	}
	close_unused_pipes(pipes, n_of_cmds - 1, -1);
	wait_for_childs(pids, n_of_cmds, tmpfile);
}

void	handle_pipes(t_commands *cmds, char *tmpfile, int n_of_cmds, char **env)
{
	// int n_of_pipes;

	// n_of_pipes = calculate_number_of_pipes(av);
	execute_pipes(cmds, n_of_cmds, tmpfile, env);
}