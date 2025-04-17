/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:37:40 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/13 11:51:21 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *get_tmp_file(void)
{
	char	*file_name;
	char	*tmpfile;
	int	 i;

	file_name = ft_itoa((unsigned long long) &i);
	tmpfile = ft_strjoin("/tmp/", file_name);
	free(file_name);
	return (tmpfile);
}

int	check_for_here_doc(char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (ft_strncmp("<<", av[i], 2) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0 ||
			ft_strcmp(cmd, "cd") == 0 ||
			ft_strcmp(cmd, "pwd") == 0 ||
			ft_strcmp(cmd, "export") == 0 ||
			ft_strcmp(cmd, "unset") == 0 ||
			ft_strcmp(cmd, "env") == 0 ||
			ft_strcmp(cmd, "exit") == 0);
}