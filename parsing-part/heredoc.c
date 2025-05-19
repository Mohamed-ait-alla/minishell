/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 20:20:59 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/19 18:19:36 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	write_in_here_doc_file(t_redirections *redir, t_env *env, int fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->file) == 0)
		{
			if (!line)
				printf("minishell: warning: here-document "
					"delimited by end-of-file (wanted `%s')\n",
					redir->file);
			break ;
		}
		if (redir->quote_type == NO_QUOTE)
			line = expand_the_heredoc(line, env);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}

static void	handle_child_heredoc(t_commands *cmd, t_env *env, char **files,
		int start_idx)
{
	t_redirections	*redir;
	int				fd;
	int				local_idx;

	redir = cmd->redirections;
	local_idx = start_idx;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC && redir->file)
		{
			fd = open(files[local_idx], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("heredoc open");
				ft_malloc(0, 0);
				exit(1);
			}
			write_in_here_doc_file(redir, env, fd);
			close(fd);
			local_idx++;
		}
		redir = redir->next;
	}
	ft_malloc(0, 0);
	exit(0);
}

static void	here_doc_process(t_commands *cmds, t_env *env, char **files)
{
	int (pid), (status), (nredir), (idx), (start_idx);
	idx = 0;
	status = 0;
	while (cmds)
	{
		nredir = count_redirections(cmds);
		if (nredir > 0)
		{
			start_idx = idx;
			pid = fork();
			if (pid < 0)
				return ;
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				handle_child_heredoc(cmds, env, files, start_idx);
			}
			ign_ctrl_c_with_exit_status(pid, &status, &cmds->signal_detected);
			cmds->here_doc_file = ft_strdup(files[start_idx + nredir - 1]);
			idx = start_idx + nredir;
		}
		if (cmds->signal_detected)
			break ;
		cmds = cmds->next;
	}
}

int	heredoc(t_commands *cmds, t_env *env)
{
	t_commands	*tmp_cmds;
	char		**files;
	int			total_here_doc;

	files = NULL;
	total_here_doc = 0;
	tmp_cmds = cmds;
	total_here_doc = count_here_doc(tmp_cmds, &files);
	if (total_here_doc > 16)
		return (-1);
	here_doc_process(tmp_cmds, env, files);
	unlink_files(total_here_doc, files);
	handle_parent_signals();
	return (0);
}
