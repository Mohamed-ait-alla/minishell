/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 09:51:14 by mait-all          #+#    #+#             */
/*   Updated: 2025/02/23 21:42:10 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_error(char *buffer, char *readed_lines)
{
	free(buffer);
	free(readed_lines);
	return (NULL);
}

static char	*ft_fill_line_buffer(int fd, char *readed_lines)
{
	ssize_t		nbytes;
	char		*buffer;
	char		*tmp;

	buffer = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	nbytes = 1;
	while (nbytes > 0)
	{
		nbytes = read(fd, buffer, BUFFER_SIZE);
		if (nbytes == -1)
			return (ft_error(buffer, readed_lines));
		buffer[nbytes] = '\0';
		if (!readed_lines)
			readed_lines = ft_strdup("");
		tmp = readed_lines;
		readed_lines = ft_strjoin(tmp, buffer);
		free(tmp);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	free(buffer);
	return (readed_lines);
}

static char	*ft_extract_line(char **readed_lines)
{
	char	*line;
	char	*tmp;
	ssize_t	i;

	if (!*readed_lines || **readed_lines == '\0')
		return (NULL);
	i = 0;
	while ((*readed_lines)[i] != '\n' && (*readed_lines)[i] != '\0')
		i++;
	line = ft_substr(*readed_lines, 0, i + ((*readed_lines)[i] == '\n'));
	tmp = ft_strdup(*readed_lines + i + ((*readed_lines)[i] == '\n'));
	free(*readed_lines);
	*readed_lines = NULL;
	*readed_lines = tmp;
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*readed_lines;
	char		*line;

	if (fd == -2)
	{
		free(readed_lines);
		return (NULL);
	}
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	readed_lines = ft_fill_line_buffer(fd, readed_lines);
	if (!readed_lines)
		return (NULL);
	line = ft_extract_line(&readed_lines);
	if (!line && readed_lines)
	{
		free(readed_lines);
		readed_lines = NULL;
	}
	return (line);
}
