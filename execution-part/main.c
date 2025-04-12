/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:07:37 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/10 13:17:47 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_for_redirections(char **av, char *tmpfile)
{
	int	i;

	i = 1;
	while (av && av[i])
	{
		if (ft_strcmp(av[i], ">") == 0) // redirect output to the specified file, overwrite it if it exist
			redirect_output_to_file(av[i + 1], 'o');
		else if (ft_strcmp(av[i], ">>") == 0) // redirect output to the specified file, append to it if it exist 
			redirect_output_to_file(av[i + 1], 'a');
		else if (ft_strcmp(av[i], "<") == 0) // redirect input to the specified file, so the command can read from it
			redirect_input_to_file(av[i + 1]);
		else if (ft_strcmp(av[i], "<<") == 0) // here_document
			redirect_input_to_file_here_doc(av[i + 1], tmpfile);
		i++;
	}
}

int	main(int ac, char **av, char **env)
{
	char	*tmpfile;
	int		pid;
	int		status;
	int		is_here_doc;

	status = 0;
	tmpfile = NULL;
	is_here_doc = check_for_here_doc(av);
	if (is_here_doc)
		tmpfile = get_tmp_file();
	// check for pipes
	if (check_for_pipes(av))
		handle_pipes(av, env, tmpfile);
	// if no pipes are included execute other commands as normal
	pid = fork();
	if (pid == -1)
			perror("fork: ");
	if (pid == 0)
	{
		check_for_redirections(av, tmpfile);
		execute_command(av[3], env);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (is_here_doc)
		{
			if (unlink(tmpfile) == -1)
				perror("unlink: ");
		}
		exit(WEXITSTATUS(status));
	}
}
