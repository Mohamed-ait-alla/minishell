/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:39:56 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/21 16:12:40 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_unset(char **args, char **env, int is_created)
{
	int	i;
	int	var_index;
	int	is_found;

	i = 1;
	while (args && args[i])
	{
		if (has_equal_sign(args[i]))
		{
			is_found = search_for_env_var(env, args[i]);
			if (is_found || (is_found && is_created))
			{
				var_index = get_env_var_index(env, args[i]);
				while (env && env[var_index])
				{
					env[var_index] = env[var_index + 1];
					var_index++;
				}
			}
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
