/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:41:21 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/30 14:48:47 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_get_env_len(char **env)
{
	int	count;

	count = 0;
	while (env && env[count])
		count++;
	return (count);
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
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_' && (arg[i] == '+' && arg[i + 1] != '='))
			return (false);
		i++;
	}
	return (true);
}

bool has_plus_sign(char *arg)
{
	if (arg[ft_strlen(arg) - 1] == '+')
		return (true);
	return (false);
}

void	add_var_to_env(t_exec_env *exec_env, char *var)
{
	char	**new_env;
	int		len;
	int		j;
	len = ft_get_env_len(exec_env->env);
	new_env = malloc(sizeof(char *) * (len + 2));
	if (!new_env)
		return ;
	j = 0;
	while (j < len)
	{
		new_env[j] = exec_env->env[j];
		j++;
	}
	new_env[j] = ft_strdup(var);
	new_env[j + 1] = NULL;
	free(exec_env->env);
	exec_env->env = new_env;
	// return (env);
}

// this function responsible for handling += feature in export
void	handle_plus_sign_feature(t_exec_env *exec_env, char *var)
{
	int		is_found;
	char	*key;
	char	*value;
	char	*new_key;
	char	*new_value;
	char	*holder;
	char	*tmp_holder_1;
	char	*tmp_holder_2;
	
	new_value = ft_strchr(var, '=') + 1;
	new_key = ft_substr(var, 0, new_value - 2 - var);
	is_found = search_for_env_var(exec_env->env, new_key);
	// if var  exists
	if (is_found)
	{
		value = ft_strchr(exec_env->env[is_found], '=') + 1;
		key = ft_substr(exec_env->env[is_found], 0, value - 1 - exec_env->env[is_found]);
		holder = ft_strjoin(value, new_value);
		tmp_holder_1 = ft_strjoin(key, "=");
		tmp_holder_2 = ft_strjoin(tmp_holder_1, holder);
		free(holder);
		free(tmp_holder_1);
		free(exec_env->env[is_found]);
		exec_env->env[is_found] = ft_strdup(tmp_holder_2);
		free(tmp_holder_2);
		free(key);
		free(new_key);
	}
	// if var do not exists	
	else
	{
		holder = ft_strjoin(new_key, "=");
		tmp_holder_1 = ft_strjoin(holder, new_value);
		add_var_to_env(exec_env, tmp_holder_1);
		free(holder);
		free(tmp_holder_1);
	}
}

int	builtin_export(char **args, t_exec_env *exec_env)
{
	int		i;
	int		is_found;
	char	**key_value;
	char	*key;
	char	*value;
	char	*has_plus;
	char	*has_equal;
	
	if (!args[1])
	{
		sort_env_vars(exec_env->env);
		i = 0;
		while (exec_env->env[i])
		{
			value = ft_strchr(exec_env->env[i], '=');
			key = ft_substr(exec_env->env[i], 0, value - exec_env->env[i]);
			if (!value)
			{
				i++;
				printf("declare -x %s\n", key);
				continue;
			}
			printf("declare -x %s=\"%s\"\n", key, value + 1);
			free(key);
			i++;
		}
	}
	i = 1;
	while (args && args[i])
	{
		if (is_valid_identifier(args[i]))
		{
			if (!has_equal_sign(args[i])) // if var has = check if it exist in env or not
			{
				// check if args[i] has + --> append mode
				if (ft_strchr(args[i], '+'))
					handle_plus_sign_feature(exec_env, args[i]); // handle += feature
				else
				{
					// handle = feature
					has_equal = ft_strchr(args[i], '=');
					key = ft_substr(args[i], 0, has_equal - args[i]);
					is_found = search_for_env_var(exec_env->env, key);
					free(key);
					if (is_found) // if var has = and exist in env update it
					{
						free(exec_env->env[is_found]);
						exec_env->env[is_found] = ft_strdup(args[i]);
					}
					else // if var has = but doesn't exist in env
						add_var_to_env(exec_env, args[i]);
				}
			}
			else // if var has no = add directly to env
				add_var_to_env(exec_env, args[i]);
		}
		else
		{
			printf("bash: export: \'%s\': not a valid identifier\n", args[i]);
			exit(EXIT_FAILURE);	
		}
		i++;
	}
	return (0);
}