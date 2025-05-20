/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin_commands.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 12:01:53 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/20 15:22:10 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_env_var_index(char **env, char *var)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp(var, env[i], ft_strlen(var)) == 0)
			return (i);
		i++;
	}
	return (0);
}

bool	search_for_env_var(char **env, char *var)
{
	int	i;
	int	len;

	i = 0;
	while (env && env[i])
	{
		len = ft_max(ft_strlen(var),
			ft_strlen(ft_substr(env[i], 0, ft_strchr(env[i], '=') - env[i])));
		if (ft_strncmp(var, env[i], len) == 0)
			return (true);
		i++;
	}
	return (false);
}

int	has_equal_sign(char *var)
{
	int	i;

	i = 0;
	while (var && var[i])
	{
		if (var[i] == '=')
			return (false);
		i++;
	}
	return (true);
}

int	execute_builtin(char **args, t_exec_env *exec_env, int last_cmd_exit_status)
{
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, exec_env->env, exec_env->is_created));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(exec_env->env));
	else if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args, last_cmd_exit_status));
	else if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(exec_env->env));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, exec_env->env, exec_env->is_created));
	else if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	else if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(args, exec_env));
	return (0);
}
