/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 22:20:43 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/08 19:43:13 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/minishell.h"

int	ft_get_env_len(char **env)
{
	int	count;

	count = 0;
	while (env && env[count])
		count++;
	return (count);
}

int	ft_get_env_var_len(char *env_var)
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

void	sort_env(char **env)
{
	int	i;
	int	j;
	int	len_a;
	int	len_b;

	i = 0;
	while (env && env[i])
	{
		len_a = ft_get_env_var_len(env[i]);
		j = i + 1;
		while (env[j])
		{
			len_b = ft_get_env_var_len(env[j]);
			if (ft_strncmp(env[i], env[j], ft_max(len_a, len_b)) > 0)
				ft_swap(&env[i], &env[j]);
			j++;
		}
		i++;
	}
}

void	print_sorted_env(t_exec_env *exec_env)
{
	int		i;
	char	*key;
	char	*value;

	sort_env(exec_env->env);
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
		// free(key);
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