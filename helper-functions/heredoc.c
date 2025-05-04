/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:20:59 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/04 16:32:50 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	heredoc(t_commands *cmd, char **env)
{
	t_commands	*tmp_cmd;
	char		*heredoc_input;
	char		*file_name;
	int			i;
	int			fd;

	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		i = 0;
		if (tmp_cmd->heredoc)
		{
			while (tmp_cmd->input_file && tmp_cmd->input_file[i])
			{
				file_name = get_tmp_file();
				fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd < 0)
				{
					perror("failed to open temporary file: ");
					return (-1);
				}
				while (1)
				{
					heredoc_input = readline("> ");
					if (!heredoc_input || ft_strcmp(heredoc_input, tmp_cmd->input_file[i]) == 0)
						break;
					write(fd, heredoc_input, ft_strlen(heredoc_input));
					write(fd, "\n", 1);
					free(heredoc_input);
				}
				free(heredoc_input);
				close(fd);

				fd = open(file_name, O_RDONLY);
				if (fd < 0)
				{
					perror("file_name: ");
					return (-1);
				}
				tmp_cmd->fds_of_heredoc[i] = fd;
				// unlink(file_name);
				free(file_name);
				i++;
			}
			tmp_cmd->fds_of_heredoc[i] = -1;
		}
		tmp_cmd = tmp_cmd->next;
	}
	return (0);
}
