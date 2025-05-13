/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:20:59 by mdahani           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/05/13 11:24:02 by mdahani          ###   ########.fr       */
=======
/*   Updated: 2025/05/12 16:05:13 by mait-all         ###   ########.fr       */
>>>>>>> 4f5c6f34f90bf5c513f78778086c76772804c225
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
	return (new_str);
}

char	*expand_the_heredoc(char *input_heredoc, t_commands *cmd_list,
		t_env *env)
{
	char	*result;

	int i, start;
	i = 0, start = 0;
	result = ft_strdup("");
	char *key, *value, *tmp;
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
			result = tmp;
		}
		else
		{
			result = ft_strjoin_char(result, input_heredoc[i]);
			i++;
		}
	}
	return (result);
}

static int	handle_child_heredoc(t_commands *cmd, t_env *env, char **files)
{
	int		fd;
	int		i;
	char	*heredoc_input;

	i = 0;
	handle_here_doc_signals();
	while (cmd->input_file && cmd->input_file[i])
	{
		fd = open(files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("failed to open temporary file");
			exit(1);
		}
		while (1)
		{
			heredoc_input = readline("> ");
			if (!heredoc_input || ft_strcmp(heredoc_input,
					cmd->input_file[i]) == 0)
			{
				if (!heredoc_input)
					printf("minishell: warning: here-document at line "
							"%d delimited by end-of-file (wanted `%s')\n",
							i + 1,
							cmd->input_file[i]);
				break ;
			}
			if (cmd->quote_type == NO_QUOTE)
				heredoc_input = expand_the_heredoc(heredoc_input, cmd, env);
			write(fd, heredoc_input, ft_strlen(heredoc_input));
			write(fd, "\n", 1);
		}
		close(fd);
		i++;
	}
	exit(0);
}

int	heredoc(t_commands *cmd, t_env *env)
{
	t_commands	*tmp;
	t_commands	*iter;
	int			count;
	char		**heredoc_files;

	tmp = cmd;
	iter = cmd;
	count = 0;
	heredoc_files = NULL;
	int pid, status, i = 0;
	while (iter)
	{
		if (iter->heredoc)
		{
			while (iter->input_file && iter->input_file[i])
			{
				heredoc_files = ft_realloc_array(heredoc_files, get_tmp_file());
				count++;
				i++;
			}
		}
		iter = iter->next;
	}
	if (count > 16)
		return (-1);
	while (tmp)
	{
		if (tmp->heredoc)
		{
			pid = fork();
			if (pid == 0)
				handle_child_heredoc(tmp, env, heredoc_files);
			signal(SIGINT, SIG_IGN);
			waitpid(pid, &status, 0);
			g_exit_status = WEXITSTATUS(status);
			if (WIFSIGNALED(status))
				g_exit_status = 130;
		}
		i = 0;
		while (heredoc_files && heredoc_files[i + 1])
		{
			unlink(heredoc_files[i]);
			i++;
		}
		tmp->here_doc_fd = open(heredoc_files[count - 1], O_RDONLY);
		if (tmp->here_doc_fd < 0)
		{
			perror("failed to open temporary file");
			return (-1);
		}
		tmp = tmp->next;
	}
	handle_parent_signals();
	return (0);
}
