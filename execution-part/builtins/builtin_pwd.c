/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:41:17 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/21 12:10:01 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(char **env)
{
	int		pwd_index;
	char	*cwd;

	if (search_for_env_var(env, "PWD"))
	{
		pwd_index = get_env_var_index(env, "PWD");
		if ((env[pwd_index] + 4)[0] != '\0')
			printf("%s\n", env[pwd_index] + 4);
		else
		{
			cwd = getcwd(NULL, 0);
			printf("%s\n", cwd);
			free(cwd);
		}
	}
	else
	{
		cwd = getcwd(NULL, 0);
		printf("%s\n", cwd);
		free(cwd);
	}
	return (EXIT_SUCCESS);
}
