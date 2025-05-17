/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:39:17 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/17 10:19:48 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_set_env(char **env, char *key, char *value, int is_created)
{
	char	*key_value;
	int		is_found;

	is_found = search_for_env_var(env, key);
	if (is_found || (!is_found && is_created))
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

static int	change_dir_to_home(char **env, char *home, int is_created)
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
	ft_set_env(env, "PWD=", cwd, is_created);
	return (EXIT_SUCCESS);
}

static int	change_dir_to_path(char **env, char *path,
								char *old_path, int is_created)
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
	ft_set_env(env, "PWD=", cwd, is_created);
	if (!is_created)
		ft_set_env(env, "OLDPWD=", old_path, is_created);
	return (EXIT_SUCCESS);
}

int	builtin_cd(char **args, char **env, int is_created)
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
		return (change_dir_to_home(env, home, is_created));
	return (change_dir_to_path(env, args[1], old_pwd, is_created));
}
