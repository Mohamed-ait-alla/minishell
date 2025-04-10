/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:40:24 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/10 13:13:42 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool    check_for_pipes(char **av)
{
    int i;

    i = 0;
    while (av[i])
    {
        if (av[i][0] == '|')
            return (true);
        i++;
    }
    return (false);
}

int calculate_number_of_pipes(char **av)
{
    int i;
    int counter;

    i = 0;
    counter = 0;
    while (av[i])
    {
        if (av[i][0] == '|')
            counter++;
        i++;
    }
    return (counter);
}

static void    close_unused_pipes(int pipes[][2], int n_of_pipes, int except)
{
    int i;

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

void    execute_pipes(char **av, int n_of_pipes, char **env, char *tmpfile)
{
    int pids[n_of_pipes + 1];
    int pipes[n_of_pipes][2];
	int status;
    int i;
    int j;
    
	status = 0;
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
    // start the while loop to fork process
    i = 0;
    while (i < n_of_pipes + 1)
    {
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("an error occured while forking processes: ");
            exit (EXIT_FAILURE);
        }
        if (pids[i] == 0) // child process
        {
            if (i == 0) // first command 
            {
				check_for_redirections(av, tmpfile);
                redirect_output_to_pipe(pipes[i][1]);
                j = 3;
            }
            else if (i == n_of_pipes) // last command
            {
				redirect_input_to_pipe(pipes[i - 1][0]);
				check_for_redirections(av, tmpfile); // take care when here_doc is found, will redirect input again
                j = 2; // j here just for testing when there more than 2 pipes increment with 3 if less increment with 2 and so on
				if (n_of_pipes > 2)
					j = 4;
            }
            else // middle commands
            {
                redirect_input_to_pipe(pipes[i - 1][0]);
                redirect_output_to_pipe(pipes[i][1]);
                j = 2;
				if (n_of_pipes > 2)
					j = 3;
            }
            close_unused_pipes(pipes, n_of_pipes, -1);
            execute_command(av[i + j], env);
        }
        i++;
    }
    close_unused_pipes(pipes, n_of_pipes, -1);
    i = 1;
    while (i < n_of_pipes + 1)
    {
        waitpid(pids[i], &status, 0);
		if (WIFEXITED(status) && (i == n_of_pipes))
		{
			if (tmpfile)
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

void    handle_pipes(char **av, char **env, char *tmpfile)
{
    int n_of_pipes;

    n_of_pipes = calculate_number_of_pipes(av);
	execute_pipes(av, n_of_pipes, env, tmpfile);
}