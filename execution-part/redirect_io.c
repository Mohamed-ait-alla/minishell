/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_io.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:32:45 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/18 11:53:33 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	redirect_output_to_file(t_commands *cmds, int is_builtin,
								int *exit_status, int *has_return)
{
	int		fd;
	char	*file;

	file = cmds->redirections->file;
	if (has_return && !is_builtin)
		*has_return = -2;
	if (!file || !file[0] || (cmds->quote_type == NO_QUOTE && has_space(file)))
		return (handle_ambigous_redirect(is_builtin, has_return, exit_status));
	fd = open(file, set_output_flags(cmds), 0644);
	if (fd < 0)
		return (handle_open_errors(cmds, is_builtin, has_return, exit_status));
	if (is_builtin)
		*has_return = -1;
	dup2(fd, STDOUT_FILENO);
	close (fd);
	return (0);
}

int	redirect_input_to_file(t_commands *cmds, int is_builtin,
							int *exit_status, int *has_return)
{
	int		fd;
	char	*file;

	file = cmds->redirections->file;
	if (has_return && !is_builtin)
		*has_return = -2;
	if (!file || !file[0] || (cmds->quote_type == NO_QUOTE && has_space(file)))
		return (handle_ambigous_redirect(is_builtin, has_return, exit_status));
	fd = open (file, O_RDONLY);
	if (fd < 0)
		return (handle_open_errors(cmds, is_builtin, has_return, exit_status));
	dup2(fd, STDIN_FILENO);
	close (fd);
	return (0);
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

void	redirect_input_to_file_here_doc(char *heredoc_file)
{
	int	fd;

	fd = open (heredoc_file, O_RDONLY);
	if (fd < 0)
		return ;
	if (unlink(heredoc_file) == -1)
	{
		perror("error occured while removing here_doc file: ");
		return ;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}
