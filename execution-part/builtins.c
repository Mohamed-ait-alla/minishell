/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 12:01:53 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/20 12:51:48 by mait-all         ###   ########.fr       */
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

int	builtin_pwd(char **env)
{
	int	pwd_index;

	pwd_index = search_for_env_var(env, "PWD");
	if (pwd_index)
	{
		printf("%s\n", env[pwd_index] + 4);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int	is_numeric(char *arg)
{
	int	i;

	i = 0;
	while (arg && arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	builtin_exit(char	**args)
{
	if (!args[1])
		exit(EXIT_SUCCESS); // exit the last cmd's status
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("bash: exit: numeric argument required\n", STDERR_FILENO);
		exit(255);
	}
	if (args[2])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", STDERR_FILENO);
		return ;
	}
	exit (ft_atoi(args[1]));
}

int	builtin_env(char **env)
{
	int	i;
	
	i = 0;
	while (env && env[i])
	{
		if (ft_strchr(env[i], '='))
			printf("%s\n", env[i]);
		i++;
	}
	if (i != 0)
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
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
}