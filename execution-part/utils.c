/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:37:40 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/05 19:15:00 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_tmp_file(void)
{
	char		*file_name;
	char		*tmpfile;
	static int	i;
	char		*heredoc_name;

	heredoc_name = "heredoc_";
	file_name = ft_strjoin(heredoc_name, ft_itoa(i));
	tmpfile = ft_strjoin("/tmp/", file_name);
	free(file_name);
	i++;
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
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

char	**copy_env(char **envp)
{
	char	**new_env;
	int		i;

	i = 0;
	while (envp && envp[i])
		i++;
	new_env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!new_env)
	{
		perror("malloc: ");
		return (NULL);
	}
	i = 0;
	while (envp && envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			perror("ft_strdup: ");
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	update_shell_level(t_exec_env *exec_env, int mode)
{
	char	*tmp;
	int		value;
	char	*new_value;
	int		is_found;

	is_found = search_for_env_var(exec_env->env, "SHLVL");
	if (is_found)
	{
		value = ft_atoi(ft_strchr(exec_env->env[is_found], '=') + 1);
		if (mode) // new program runed so increment shell level
			++value;
		else
			--value;
		new_value = ft_itoa(value);
		tmp = ft_strjoin("SHLVL=", new_value);
		free(exec_env->env[is_found]);
		exec_env->env[is_found] = ft_strdup(tmp);
		free(new_value);
		free(tmp);
	}
}

void	free_double_array(char **arr)
{
	int i;

	i = 0;
	while (arr && arr[i])
		free(arr[i++]);
	free(arr);
}