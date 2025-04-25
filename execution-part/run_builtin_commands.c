/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin_commands.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 12:01:53 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/25 20:00:28 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int	search_for_env_var(char **env, char *var)
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

int	execute_builtin(char **args, char **env)
{
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, env));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd(env));
	else if (ft_strcmp(args[0], "exit") == 0)
		builtin_exit(args);
	else if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(env));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, env));
	else if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args, env));
	else if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(args, env));
}