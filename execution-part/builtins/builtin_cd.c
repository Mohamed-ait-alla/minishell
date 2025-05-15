/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:39:17 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/15 20:21:22 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_set_env(char **env, char *key, char *value)
{
	char	*key_value;
	int		is_found;

	is_found = search_for_env_var(env, key);
	if (is_found)
	{
		key_value = ft_strjoin(key, value);
		env[is_found] = ft_strdup(key_value);
		if (!env[is_found])
		{
			perror ("ft_strdup: ");
			return ;
		}
	}
}

static int	change_dir_to_home(char **env, char *home)
{
	char	cwd[PATH_MAX];

	if (!home)
	{
		printf("home is not setted\n");
		return (EXIT_FAILURE);
	}
	if (chdir(home) != 0)
	{
		perror("chdir: ");
		return (EXIT_FAILURE);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd: ");
		return (EXIT_FAILURE);
	}
	ft_set_env(env, "PWD=", cwd);
	return (EXIT_SUCCESS);
}

static int	change_dir_to_path(char **env, char *path, char *old_path)
{
	char	cwd[PATH_MAX];

	if (chdir(path) != 0)
	{
		perror("chdir: ");
		return (EXIT_FAILURE);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd: ");
		return (EXIT_FAILURE);
	}
	ft_set_env(env, "PWD=", cwd);
	ft_set_env(env, "OLDPWD=", old_path);
	return (EXIT_SUCCESS);
}

int	builtin_cd(char **args, char **env)
{
	char	*old_pwd;
	char	*home;

	if (args && args[2])
	{
		printf("minishell: cd: too many arguments\n");
		return (EXIT_FAILURE);
	}
	old_pwd = getenv("PWD");
	home = getenv("HOME");
	if (!args || !args[0])
		return (EXIT_FAILURE);
	if (!args[1])
		return (change_dir_to_home(env, home));
	return (change_dir_to_path(env, args[1], old_pwd));
}
