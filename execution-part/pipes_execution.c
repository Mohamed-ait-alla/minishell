/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 10:41:13 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/12 10:57:19 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

static void	configure_pipeline_io(char **av, int pipes[][2], int i, int *j, int n_of_pipes, char *tmpfile)
{
	if (i == 0) // first command 
	{
		check_for_redirections(av, tmpfile);
		redirect_output_to_pipe(pipes[i][1]);
		*j = 3;
	}
	else if (i == n_of_pipes) // last command
	{
		redirect_input_to_pipe(pipes[i - 1][0]);
		check_for_redirections(av, tmpfile); // take care when here_doc is found
		*j = 2; // j here just for testing when there more than 2 pipes increment with 3 if less increment with 2 and so on
		if (n_of_pipes > 2)
			*j = 4;
	}
	else // middle commands
	{
		redirect_input_to_pipe(pipes[i - 1][0]);
		redirect_output_to_pipe(pipes[i][1]);
		*j = 2;
		if (n_of_pipes > 2)
			*j = 3;
	}
}

static void	wait_for_childs(int pids[], int n_of_pipes, char *tmpfile)
{
	int	status;
	int	i;

	status = 0;
	i = 0;
	while (i < n_of_pipes + 1)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status) && (i == n_of_pipes))
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

static void	execute_pipes(char **av, int n_of_pipes, char **env, char *tmpfile)
{
	int pids[n_of_pipes + 1];
	int pipes[n_of_pipes][2];
	int i;
	int j;

	i = 0;
	// creates the pipe ends
	while (i < n_of_pipes)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("an error occured while creating pipes: ");
			exit (EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	while (i < n_of_pipes + 1)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("an error occured while forking processes: ");
			exit (EXIT_FAILURE);
		}
		if (pids[i] == 0) // child processes
		{
			configure_pipeline_io(av, pipes, i, &j, n_of_pipes, tmpfile);
			close_unused_pipes(pipes, n_of_pipes, -1);
			printf("j= %d\n", j);
			execute_command(av[i + j], env);
		}
		i++;
	}
	close_unused_pipes(pipes, n_of_pipes, -1);
	wait_for_childs(pids, n_of_pipes, tmpfile);
}

void	handle_pipes(char **av, char **env, char *tmpfile)
{
	int n_of_pipes;

	n_of_pipes = calculate_number_of_pipes(av);
	execute_pipes(av, n_of_pipes, env, tmpfile);
}