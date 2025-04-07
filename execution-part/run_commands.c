/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:14:22 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/07 10:01:38 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_path(char **env)
{
		char	*env_path;
		int	 i;

		i = 0;
		while (env && env[i])
		{
				if (ft_strncmp("PATH=", env[i], 5) == 0)
						return (env[i]);
				i++;
		}
		return (NULL);
}  

static char	*get_path_name(char *cmd, char **env)
{
		char	*env_path;
		char	*holder;
		char	*binary_path;
		char	**exec_dirs;
		int		i;

		env_path = get_env_path(env);
		exec_dirs = ft_split(env_path, ':');
		i = 0;
		while (exec_dirs && exec_dirs[i])
		{
				holder = ft_strjoin(exec_dirs[i], "/");
				binary_path = ft_strjoin(holder, cmd);
				free(holder);
				if (access(binary_path, F_OK) == 0 && access(binary_path, X_OK) == 0)
						return (binary_path);
				free(binary_path);
				i++;
		}
		return (NULL);
}

void	execute_command(char *cmd, char **env)
{
	char	**args;
	char	*path_name;

	args = ft_split(cmd, ' ');
	path_name = get_path_name(args[0], env);
	if (!path_name)
	{
			printf("no binary path found for this command\n");
			exit(EXIT_FAILURE);
	}
	execve(path_name, args, env);
	perror("execve: ");
	exit(EXIT_FAILURE);
}