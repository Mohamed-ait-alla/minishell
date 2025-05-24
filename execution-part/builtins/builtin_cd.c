/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:39:17 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/23 11:32:46 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_set_env(char **env, char *key, char *value, int is_created)
{
	char	*key_value;
	int		is_found;
	int		var_index;

	is_found = search_for_env_var(env, key);
	if (is_found || (!is_found && is_created))
	{
		var_index = get_env_var_index(env, key);
		key_value = ft_strjoin(key, value);
		env[var_index] = ft_strdup(key_value);
		if (!env[var_index])
		{
			perror ("ft_strdup: ");
			return ;
		}
	}
}

static	int	get_cd_args_len(char **args)
{
	int	i;

	i = 1;
	while (args && args[i])
		i++;
	return (i);
}

static int	change_dir_to_home(char **env, char *home, int is_created)
{
	char	cwd[PATH_MAX];

	if (!home || (home && !search_for_env_var(env, "HOME")))
	{
		ft_putstr_fd("bash: cd: HOME not set\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	if (!(ft_strchr(env[get_env_var_index(env, "HOME")], '=') + 1)[0])
		return (EXIT_SUCCESS);
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
	if (!is_created)
		ft_set_env(env, "PWD=", cwd, is_created);
	if (!is_created)
		ft_set_env(env, "OLDPWD=", old_path, is_created);
	return (EXIT_SUCCESS);
}

int	builtin_cd(char **args, char **env, int is_created)
{
	char	*old_pwd;
	char	*home;

	if (get_cd_args_len(args) > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
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
