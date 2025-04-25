/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 12:01:53 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/25 17:54:35 by mait-all         ###   ########.fr       */
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
		exit(EXIT_SUCCESS); // exit with the last cmd's status
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

int	builtin_unset(char **args, char **env)
{
	int	i;
	int	is_found;

	i = 1;
	while (args && args[i])
	{
		if (has_equal_sign(args[i]))
		{
			is_found = search_for_env_var(env, args[i]);
			if (is_found)
			{
				free(env[is_found]);
				while (env && env[is_found])
				{
					env[is_found] = env[is_found + 1];
					is_found++;
				}
			}
		}
		i++;
	}
	if (i == 1)
		return (EXIT_FAILURE);
	else
		return (EXIT_SUCCESS);
}

int	ft_env_len(char **env)
{
	int	count;

	count = 0;
	while (env && env[count])
		count++;
	return (count);
}

int	builtin_echo(char **args, char **env)
{
	int	i;
	int	offset;
	int	is_option;
	int	is_found;

	i = 1;
	is_option = 0;
	offset = 1;
	if (!args[1])
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	if (ft_strncmp(args[1], "-n", 2) == 0)
	{
		i = 2;
		is_option = 1;
		offset = 2;
	}
	while (args[i])
	{
		if (i != offset)
			ft_putstr_fd(" ", STDOUT_FILENO);
		if (args[i][0] == '$')
		{
			if ((is_found = search_for_env_var(env, args[i] + 1)))
				ft_putstr_fd(env[is_found] + ft_strlen(args[i]), STDOUT_FILENO);
			else
			{
				if (is_option == 1)
				{
					i++;
					continue;
				}
				ft_putstr_fd("\n", STDOUT_FILENO);
			}
		}
		else
			ft_putstr_fd(args[i], STDOUT_FILENO);
		i++;
	}
	if (!is_option)
		ft_putstr_fd("\n", STDOUT_FILENO);
}

int	get_env_var_len(char *env_var)
{
	int	count;

	count = 0;
	while (env_var[count] && env_var[count] != '=')
		count++;
	return (count);
}

int ft_max(int value1, int value2)
{
	if (value1 > value2)
		return (value1);
	return (value2);
}

void	ft_swap(char **s1, char **s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = *s2;
	*s2 = tmp;	
}

void	sort_env_vars(char **env)
{
	int	i;
	int	j;
	int	len_a;
	int	len_b;

	i = 0;
	while (env && env[i])
	{
		len_a = get_env_var_len(env[i]);
		j = i + 1;
		while (env[j])
		{
			len_b = get_env_var_len(env[j]);
			if (ft_strncmp(env[i], env[j], ft_max(len_a, len_b)) > 0)
				ft_swap(&env[i], &env[j]);
			j++;
		}
		i++;
	}
}

bool	is_valid_identifier(char *arg)
{
	int	i;

	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (false);
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]))
			return (false);
		i++;
	}
	return (true);
}

int	builtin_export(char **args, char **env)
{
	int		i;
	int		j;
	int		is_found;
	int		len;
	char	**key_value;
	char	**new_env;
	
	if (!args[1])
	{
		sort_env_vars(env);
		i = 0;
		while (env[i])
		{
			key_value = ft_split(env[i], '=');
			if (!key_value[1])
			{
				i++;
				printf("declare -x %s\n", key_value[0]);
				continue;
			}
			printf("declare -x %s=\"%s\"\n", key_value[0], key_value[1]);
			free_double_array(key_value);
			i++;
		}
	}
	i = 1;
	while (args && args[i])
	{
		if (is_valid_identifier(args[i]))
		{
			if (!has_equal_sign(args[i]))
			{
				key_value = ft_split(args[i], '=');
				is_found = search_for_env_var(env, key_value[0]);
				if (is_found)
				{
					free(env[is_found]);
					env[is_found] = ft_strdup(args[i]);
				}
				else
				{
					len = ft_env_len(env);
					new_env = malloc(sizeof(char *) * (len + 2));
					if (!new_env)
						return (0);
					j = 0;
					while (j < len)
					{
						new_env[j] = env[j];
						j++;
					}
					new_env[len] = ft_strdup(args[i]);
					new_env[len + 1] = NULL;
					free(env);
					env = new_env;
				}
			}
			else
			{
				len = ft_env_len(env);
				new_env = malloc(sizeof(char *) * (len + 2));
				if (!new_env)
					return (EXIT_FAILURE);
				j = 0;
				while (j < len)
				{
					new_env[j] = env[j];
					j++;
				}
				new_env[j] = ft_strdup(args[i]);
				new_env[j + 1] = NULL;
				free(env);
				env = new_env;
			}
		}
		i++;
	}
	return (0);
}

int	execute_builtin(char **args, char **env, t_command **cmds)
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