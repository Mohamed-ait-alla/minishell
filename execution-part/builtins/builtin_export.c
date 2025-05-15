/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:41:21 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/15 18:34:32 by mait-all         ###   ########.fr       */
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
	// free(exec_env->env);
	exec_env->env = new_env;
}

void	append_env_var(t_exec_env *exec_env, char *new_key, char *new_value, int is_found)
{
	char	*key;
	char	*value;
	char	*holder;
	char	*tmp_1;
	char	*tmp_2;

	value = ft_strchr(exec_env->env[is_found], '=') + 1;
	key = ft_substr(exec_env->env[is_found], 0, value - 1 - exec_env->env[is_found]);
	holder = ft_strjoin(value, new_value);
	tmp_1 = ft_strjoin(key, "=");
	tmp_2 = ft_strjoin(tmp_1, holder);
	// free(holder);
	// free(tmp_1);
	// free(exec_env->env[is_found]);
	exec_env->env[is_found] = ft_strdup(tmp_2);
	// free(tmp_2);
	// free(key);
	// free(new_key);
}

// this function responsible for handling += feature in export
void	handle_plus_sign_feature(t_exec_env *exec_env, char *var)
{
	int		is_found;
	char	*new_key;
	char	*new_value;
	char	*holder;
	char	*tmp;
	
	new_value = ft_strchr(var, '=') + 1;
	new_key = ft_substr(var, 0, new_value - 2 - var);
	is_found = search_for_env_var(exec_env->env, new_key);
	if (is_found)
		append_env_var(exec_env, new_key, new_value, is_found);
	else	
	{
		holder = ft_strjoin(new_key, "=");
		tmp = ft_strjoin(holder, new_value);
		add_var_to_env(exec_env, tmp);
		// free(holder);
		// free(tmp);
	}
}

void	handle_equal_sign_feature(t_exec_env *exec_env, char *var)
{
	int		is_found;
	char	*key;

	key = ft_substr(var, 0, ft_strchr(var, '=') - var);
	is_found = search_for_env_var(exec_env->env, key);
	// free(key);
	if (is_found) // if var has = and exist in env update it
	{
		// free(exec_env->env[is_found]);
		exec_env->env[is_found] = ft_strdup(var);
	}
	else // if var has = but doesn't exist in env add it
		add_var_to_env(exec_env, var);
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
		{
			if (!has_equal_sign(args[i]))
			{
				// check if args[i] has + --> append mode
				if (ft_strchr(args[i], '+'))
					handle_plus_sign_feature(exec_env, args[i]);
				// handle = feature
				else
					handle_equal_sign_feature(exec_env, args[i]);
			}
			else // if var has no = add directly to env
				add_var_to_env(exec_env, args[i]);
		}
		else
		{
			printf("minishell: export: `%s\': not a valid identifier\n", args[i]);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}