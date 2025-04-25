/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:39:17 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/25 19:42:43 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_set_env(char **env, char *key, char *value)
{
	char	*key_value;
	int		is_found;

	is_found = search_for_env_var(env, key);
	if (is_found)
	{
		key_value = ft_strjoin(key, value);
		free(env[is_found]);
		env[is_found] = ft_strdup(key_value);
		if (!env[is_found])
		{
			perror ("ft_strdup: ");
			return (0);
		}
		free(key_value);
		return (1);
	}
	return (0);
}

int	builtin_cd(char **args, char **env)
{
	char	*old_pwd;
	char	cwd[PATH_MAX];

	old_pwd = getenv("PWD");
	if (!args || !args[0])
		return (EXIT_FAILURE);
	if (chdir(args[1]) != 0)
	{
		perror("cd: ");
		return (EXIT_FAILURE);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd: ");
		return (EXIT_FAILURE);
	}
	ft_set_env(env, "PWD=", cwd);
	ft_set_env(env, "OLDPWD=", old_pwd);
	return (EXIT_SUCCESS);
}