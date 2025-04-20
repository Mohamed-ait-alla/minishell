/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:07:37 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/19 12:24:46 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_for_redirections(t_command **cmds, char *tmpfile)
{
	int	i;

	i = 0;
	while (cmds && cmds[i])
	{
		if (cmds[i]->infile) 
		{
			if (cmds[i]->heredoc)
				redirect_input_to_file_here_doc(cmds[i]->infile, tmpfile);
			else
				redirect_input_to_file(cmds[i]->infile);
		}
		if (cmds[i]->outfile)
		{
			if (cmds[i]->append)
				redirect_output_to_file(cmds[i]->outfile, 'a');
			else
				redirect_output_to_file(cmds[i]->outfile, 'o');		
		}
		i++;
	}
}

void	tested_main_with_parsing(t_command **cmds, int n_of_cmds)
{
	char	*tmpfile;
	int		pid;
	int		status;

	status = 0;
	tmpfile = NULL;
	if (cmds[0]->heredoc)
		tmpfile = get_tmp_file();
	// check for buit-ins
	if (is_builtin(cmds[0]->args[0]))
	{
		status = execute_builtin(cmds[0]->args, cmds[0]->env);
		// exit(status);
	}
	// check for pipes
	if (n_of_cmds > 1)
		handle_pipes(cmds, tmpfile, n_of_cmds);
	// if no pipes are included execute other commands as normal
	pid = fork();
	if (pid == -1)
			perror("fork: ");
	if (pid == 0)
	{
		check_for_redirections(cmds, tmpfile);
		execute_command(cmds[0]->args, cmds[0]->env);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (cmds[0]->heredoc)
		{
			printf("tmpfile removed is %s\n", tmpfile);
			if (unlink(tmpfile) == -1)
				perror("unlink: ");
		}
		// exit(WEXITSTATUS(status));
	}
}
