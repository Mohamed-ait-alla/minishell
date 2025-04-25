/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:39:56 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/25 19:45:58 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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