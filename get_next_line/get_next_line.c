/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 09:34:59 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/11 10:01:46 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*fill_line_buffer(int fd, char *remainder, char *buffer);
static char	*get_remainder(char *line);

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*line;
	char		*buffer;

	buffer = ft_malloc((BUFFER_SIZE + 1) * sizeof(char), 1);
	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(remainder);
		free(buffer);
		remainder = NULL;
		buffer = NULL;
		return (NULL);
	}
	if (!buffer)
		return (NULL);
	line = fill_line_buffer(fd, remainder, buffer);
	// free(buffer);
	buffer = NULL;
	if (!line)
		return (NULL);
	remainder = get_remainder(line);
	return (line);
}

static char	*get_remainder(char *line)
{
	char	*remainder;
	ssize_t	i;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	if (line[i] == 0 || line[1] == 0)
		return (NULL);
	remainder = ft_substr(line, i + 1, ft_strlen(line) - i);
	if (remainder[0] == 0)
	{
		// free(remainder);
		remainder = NULL;
	}
	line[i + 1] = 0;
	return (remainder);
}

static char	*fill_line_buffer(int fd, char *remainder, char *buffer)
{
	ssize_t	char_read;
	char	*tmp;

	while (1)
	{
		char_read = read(fd, buffer, BUFFER_SIZE);
		if (char_read == -1)
		{
			free(remainder);
			return (NULL);
		}
		else if (char_read == 0)
			break ;
		buffer[char_read] = 0;
		if (!remainder)
			remainder = ft_strdup("");
		tmp = remainder;
		remainder = ft_strjoin(tmp, buffer);
		// free(tmp);
		tmp = NULL;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (remainder);
}
