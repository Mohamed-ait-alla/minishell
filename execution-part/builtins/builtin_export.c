/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:41:21 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/20 10:07:15 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_plus_sign_feature(t_exec_env *exec_env, char *var)
{
	char	*new_key;
	char	*new_value;
	char	*holder;
	char	*tmp;

	new_value = ft_strchr(var, '=') + 1;
	new_key = ft_substr(var, 0, new_value - 2 - var);
	if (search_for_env_var(exec_env->env, new_key))
		append_env_var(exec_env, new_value,
			get_env_var_index(exec_env->env, new_key));
	else
	{
		holder = ft_strjoin(new_key, "=");
		tmp = ft_strjoin(holder, new_value);
		add_var_to_env(exec_env, tmp);
	}
}

void	handle_equal_sign_feature(t_exec_env *exec_env, char *var)
{
	int		is_found;
	int		index;
	char	*key;

	key = ft_substr(var, 0, ft_strchr(var, '=') - var);
	is_found = search_for_env_var(exec_env->env, key);
	if (is_found)
	{
		index = get_env_var_index(exec_env->env, key);
		exec_env->env[index] = ft_strdup(var);
	}
	else
		add_var_to_env(exec_env, var);
}

static void	process_export_arguments(t_exec_env *exec_env, char *arg)
{
	if (!has_equal_sign(arg))
	{
		if (ft_strchr(arg, '+'))
			handle_plus_sign_feature(exec_env, arg);
		else
			handle_equal_sign_feature(exec_env, arg);
	}
	else
	{
		if (!search_for_env_var(exec_env->env, arg))
			add_var_to_env(exec_env, arg);
	}
}

int	builtin_export(char **args, t_exec_env *exec_env)
{
	int		i;

	if (!args[1])
		print_sorted_env(exec_env);
	i = 1;
	while (args && args[i])
	{
		if (is_valid_identifier(args[i]))
			process_export_arguments(exec_env, args[i]);
		else
		{
			printf("minishell: export: `%s\': not a valid identifier", args[i]);
			printf("\n");
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
