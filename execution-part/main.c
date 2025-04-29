/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:07:37 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/29 11:47:18 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_for_redirections(t_commands *cmds, char *tmpfile)
{
	t_commands *tmp;

	tmp = cmds;
	while (tmp)
	{
		if (tmp->input_file) 
		{
			if (tmp->heredoc)
				redirect_input_to_file_here_doc(tmp->input_file, tmpfile);
			else
				redirect_input_to_file(tmp->input_file);
		}
		if (tmp->output_file)
		{
			if (tmp->append)
				redirect_output_to_file(tmp->output_file, 'a');
			else
				redirect_output_to_file(tmp->output_file, 'o');		
		}
		tmp = tmp->next;
	}
}

int	count_n_of_cmds(t_commands *cmds)
{
	int count;
	t_commands *tmp;

	count = 0;
	tmp = cmds;
	while (tmp)
	{
		count ++;
		tmp = tmp->next;
	}
	return (count);
}

void	tested_main_with_parsing(t_commands *cmds, t_exec_env *exec_env)
{
	char	*tmpfile;
	int		pid;
	int		status;
	int		n_of_cmds;
	
	status = 0;
	n_of_cmds = count_n_of_cmds(cmds);
	tmpfile = NULL;
	if (cmds->heredoc)
		tmpfile = get_tmp_file();
		// // check for buit-ins
		if (is_builtin(cmds->args[0]))
		{
			status = execute_builtin(cmds->args, exec_env);
			// exit(status);
		}
		// check for pipes
	if (n_of_cmds > 1)
		handle_pipes(cmds, tmpfile, n_of_cmds, exec_env->env);
	// if no pipes are included execute other commands as normal
	pid = fork();
	if (pid == -1)
			perror("fork: ");
	if (pid == 0)
	{
		check_for_redirections(cmds, tmpfile);
		execute_command(cmds->args, exec_env->env);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		if (cmds->heredoc)
		{
			printf("tmpfile removed is %s\n", tmpfile);
			if (unlink(tmpfile) == -1)
				perror("unlink: ");
		}
		// exit(WEXITSTATUS(status));
	}
}
