/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:41:21 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/29 16:30:00 by mait-all         ###   ########.fr       */
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

int	builtin_export(char **args, t_exec_env *exec_env)
{
	int		i;
	int		j;
	int		is_found;
	int		var_len;
	char	**key_value;
	char	**tmp_key_value;
	char	*holder;
	char	*plus;
	char	*tmp;
	char	*tmp_holder_1;
	char	*tmp_holder_2;
	
	if (!args[1])
	{
		sort_env_vars(exec_env->env);
		i = 0;
		while (exec_env->env[i])
		{
			key_value = ft_split(exec_env->env[i], '=');
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
			if (!has_equal_sign(args[i])) // if var has = check if it exist in env or not
			{
				key_value = ft_split(args[i], '=');
				if (has_plus_sign(key_value[0]))
				{
					key_value[0][ft_strlen(key_value[0]) - 1] = '\0'; // skip the + sign
					is_found = search_for_env_var(exec_env->env, key_value[0]);
					// if var  exists
					if (is_found)
					{
						tmp_key_value = ft_split(exec_env->env[is_found], '=');
						holder = ft_strjoin(tmp_key_value[1], key_value[1]);
						tmp_holder_1 = ft_strjoin(tmp_key_value[0], "=");
						tmp_holder_2 = ft_strjoin(tmp_holder_1, holder);
						free(holder);
						free(tmp_holder_1);
						free(exec_env->env[is_found]);
						exec_env->env[is_found] = ft_strdup(tmp_holder_2);
						free(tmp_holder_2);
						free_double_array(tmp_key_value);
						free_double_array(key_value);
					}
					// if var do not exists	
					else
					{
						plus = ft_strchr(args[i], '+');
						if (plus && plus[1] == '=')
						{
							var_len = plus - args[i];
							tmp = malloc(var_len + ft_strlen(plus + 1) + 1);
							if (!tmp)
								return (1);
							ft_memcpy(tmp, args[i], var_len);
							ft_memcpy(tmp + var_len, plus + 1, ft_strlen(plus+1));
							add_var_to_env(exec_env, tmp);
							free(tmp);
						}
					}
				}
				else // if var has no + sign
				{
					is_found = search_for_env_var(exec_env->env, key_value[0]);
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