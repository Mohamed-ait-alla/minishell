/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:20:59 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/10 20:26:39 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_strjoin_char(char *str, char c)
{
	char	*new_str;
	int		i;

	new_str = ft_malloc(sizeof(char) * (ft_strlen(str) + 2), 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	// free(str);
	return (new_str);
}

// expand the env variables in the heredoc
char	*expand_the_heredoc(char *input_heredoc, t_commands *cmd_list,
		t_env *env)
{
	int i, start;
	char *result, *key, *value, *tmp;
	i = 0;
	result = ft_strdup("");
	while (input_heredoc[i])
	{
		if (input_heredoc[i] == '$' && input_heredoc[i + 1]
			&& (ft_isalpha(input_heredoc[i + 1]) || input_heredoc[i
				+ 1] == '_'))
		{
			i++;
			start = i;
			while (input_heredoc[i] && (ft_isalnum(input_heredoc[i])
					|| input_heredoc[i] == '_'))
				i++;
			key = ft_substr(input_heredoc, start, i - start);
			value = get_env_value(env, key);
			if (!value)
				value = ft_strdup("");
			tmp = ft_strjoin(result, value);
			// free(result);
			result = tmp;
			// free(key);
		}
		else
		{
			result = ft_strjoin_char(result, input_heredoc[i]);
			i++;
		}
	}
	return (result);
}

int	heredoc(t_commands *cmd, t_env *env)
{
	t_commands	*tmp_cmd;
	char		*heredoc_input;
	char		*file_name;
	int			i;
	int			fd;
	t_commands	*tmp2_cmd;
	int			count_heredoc;

	// signal (SIGINT, SIG_IGN);
	// handle max heredo	
		tmp2_cmd = cmd;
		count_heredoc = 0;
		while (tmp2_cmd)
		{
			if (tmp2_cmd->heredoc)
			{
				i = 0;
				while (tmp2_cmd->input_file && tmp2_cmd->input_file[i])
				{
					i++;
					count_heredoc++;
				}
			}
			tmp2_cmd = tmp2_cmd->next;
		}
		if (count_heredoc > 16)
			return (-1);
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
							// handle_here_doc_signals();
							heredoc_input = readline("> ");
							if (!heredoc_input || ft_strcmp(heredoc_input,
									tmp_cmd->input_file[i]) == 0)
								break ;
							if (cmd->quote_type == NO_QUOTE)
								heredoc_input = expand_the_heredoc(heredoc_input,
										tmp_cmd, env);
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
					unlink(file_name);
					// free(file_name);
					i++;
				}
				tmp_cmd->fds_of_heredoc[i] = -1;
			}
			tmp_cmd = tmp_cmd->next;
		}
	return (0);
}
