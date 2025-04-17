/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 12:01:53 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/16 12:54:52 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

// int	builtin_cd(char **args, char **env)
// {
// 	int	i;

// 	i = 0;
// 	if (chdir(args[1]) == -1)
// 		perror("chdir: ");
// 	while (env && env[i])
// 	{
// 		if (ft_strncmp("PWD=", env[i], 4) == 0)
// 			ft_strlcpy()
// 	}
	
// }

int	execute_builtin(char **args, char **env)
{
	if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, env));
}