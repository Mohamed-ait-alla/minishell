/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_io.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:32:45 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/14 18:06:03 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

bool has_space(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] <= ' ')
			return (true);
		i++;
	}
	return (false);
}

void	redirect_output_to_file(t_commands *cmds, char *file, char mode, int is_builtin, int *exit_status, int *has_return)
{
	int fd;

	if (has_return && !is_builtin)
		*has_return = -2;
	if (!file || !file[0] || (cmds->quote_type == NO_QUOTE && has_space(file)))
	{
		if (is_builtin)
		{
			*exit_status = custom_error(ERR_AMBIG_REDIRECT, "$...", EXIT_FAILURE, is_builtin);
			*has_return = true;
			return ;
		}
		else
			custom_error(ERR_AMBIG_REDIRECT, "$...", EXIT_FAILURE, is_builtin);
	}
	if (mode == 'o') // overwriting mode
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (mode == 'a') // appending mode
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		if (is_builtin)
		{
			*exit_status = custom_error(ERR_PERMISSION, file, EXIT_FAILURE, is_builtin);
			*has_return = true;
			return ;
		}
		else
			custom_error(ERR_PERMISSION, file, EXIT_FAILURE, is_builtin);
	}
	if (is_builtin)
		*has_return = -1;
	dup2(fd, STDOUT_FILENO);
	close (fd);
}

void	redirect_input_to_file(t_commands *cmds, char *file, int is_builtin, int *exit_status, int *has_return)
{
	int fd;

	// if (has_return && !is_builtin)
	// 	*has_return = -2;
	if (!file || !file[0] || (cmds->quote_type == NO_QUOTE && has_space(file)))
	{
		if (is_builtin)
		{
			*exit_status = custom_error(ERR_AMBIG_REDIRECT, "$...", EXIT_FAILURE, is_builtin);
			*has_return = true;
			return ;
		}
		else
			custom_error(ERR_AMBIG_REDIRECT, "$...", EXIT_FAILURE, is_builtin);
	}
	fd = open (file, O_RDONLY);
	if (fd < 0)
	{
		if (is_builtin)
		{
			*exit_status = custom_error(ERR_PERMISSION, file, EXIT_FAILURE, is_builtin);
			*has_return = true;
			return ;
		}
		else
			custom_error(ERR_PERMISSION, file, EXIT_FAILURE, is_builtin);
	}
	dup2(fd, STDIN_FILENO);
	close (fd);
}

void	redirect_input_to_pipe(int read_pipe_end)
{
	dup2(read_pipe_end, STDIN_FILENO);
	close (read_pipe_end);
}

void	redirect_output_to_pipe(int write_pipe_end)
{
	dup2(write_pipe_end, STDOUT_FILENO);
	close (write_pipe_end);
}

void	redirect_input_to_file_here_doc(int heredoc_fd)
{
	// int	fd;

	// fd = open (here_doc_file, O_RDONLY);
	// if (fd < 0)
	// {
	// 	perror("error occured while opening here doc file: ");
	// 	return ;
	// }
	dup2(heredoc_fd, STDIN_FILENO);
	close(heredoc_fd);
}