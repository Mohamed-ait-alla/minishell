/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 22:20:43 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/19 22:35:17 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	add_var_to_env(t_exec_env *exec_env, char *var)
{
	char	**new_env;
	int		len;
	int		j;

	len = ft_get_env_len(exec_env->env);
	new_env = ft_malloc(sizeof(char *) * (len + 2), 1);
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
	exec_env->env = new_env;
}

void	append_env_var(t_exec_env *exec_env, char *new_value, int var_index)
{
	char	*key;
	char	*value;
	char	*holder;
	char	*tmp_1;
	char	*tmp_2;

	value = ft_strchr(exec_env->env[var_index], '=') + 1;
	key = ft_substr(exec_env->env[var_index], 0,
			value - 1 - exec_env->env[var_index]);
	holder = ft_strjoin(value, new_value);
	tmp_1 = ft_strjoin(key, "=");
	tmp_2 = ft_strjoin(tmp_1, holder);
	exec_env->env[var_index] = ft_strdup(tmp_2);
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
			continue ;
		}
		printf("declare -x %s=\"%s\"\n", key, value + 1);
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
		if ((!ft_isalnum(arg[i]) && arg[i] != '_'
				&& (arg[i] == '+' && arg[i + 1] != '=')) || arg[i] == '-')
			return (false);
		i++;
	}
	return (true);
}
