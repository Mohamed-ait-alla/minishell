/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_io.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 18:32:45 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/09 10:56:03 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	redirect_output_to_file(char *file, char mode)
{
	int fd;

	if (mode == 'o') // overwriting mode
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (mode == 'a') // appending mode
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		custom_error(ERR_PERMISSION, file, 1);
	dup2(fd, STDOUT_FILENO);
	close (fd);
}

void	redirect_input_to_file(char *file)
{
	int fd;

	fd = open (file, O_RDONLY);
	if (fd < 0)
		custom_error(ERR_PERMISSION, file, 1);
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

static char *get_tmp_file(void)
{
	char	*file_name;
	char	*tmpfile;
	int	 i;

	file_name = ft_itoa((unsigned long long) &i);
	tmpfile = ft_strjoin("/tmp/", file_name);
	free(file_name);
	return (tmpfile);
}

void	redirect_input_to_file_here_doc(char *limitter)
{
	char	*tmpfile;
	char	*line;
	char	*h_limitter;
	int	 fd;

	tmpfile = get_tmp_file();
	fd = open (tmpfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("failed to open temporary file: ");
		exit (EXIT_FAILURE);
	}
	write(1, "heredoc> ", 9);
	line = get_next_line(0);
	h_limitter = ft_strjoin(limitter, "\n");
	while (line && (ft_strncmp(line, h_limitter, ft_strlen(line)) != 0))
	{
		write(1, "heredoc> ", 9);
		write(fd, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	free(line);
	close (fd);
	fd = open (tmpfile, O_RDONLY);
	if (fd < 0)
	{
		perror ("failed to open temporary file for reading data: ");
		exit (EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close (fd);
}