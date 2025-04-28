/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:41:21 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/27 17:58:39 by mait-all         ###   ########.fr       */
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
					len = ft_get_env_len(env);
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
				len = ft_get_env_len(env);
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