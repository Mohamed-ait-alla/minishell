/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:41:17 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/17 11:05:25 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_pwd(char **env)
{
	int	pwd_index;

	pwd_index = search_for_env_var(env, "PWD");
	if (pwd_index)
		printf("%s\n", env[pwd_index] + 4);
	else
		printf("%s\n", getcwd(NULL, 0));
	return (EXIT_SUCCESS);
}
