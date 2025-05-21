/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:33:53 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/21 12:11:55 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	custom_error(char *err_msg, char *arg, int exit_code, int is_builtin)
{
	ft_putstr_fd(err_msg, STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (!is_builtin)
	{
		ft_malloc(0, 0);
		ft_exit(exit_code);
	}
	return (exit_code);
}

void	manage_shell_errors(char *arg, char *path)
{
	if (!arg)
	{
		ft_malloc(0, 0);
		ft_exit(EXIT_FAILURE);
	}
	if (!path)
		custom_error(ERR_CMD_NOT_FOUND, arg, 127, false);
	if (ft_strncmp(path, "no file", ft_strlen(path)) == 0)
		custom_error(ERR_NO_FILE, arg, 127, false);
}

int	handle_fork_errors(int pid)
{
	if (pid == -1)
	{
		perror("an error occured while forking processes: ");
		g_exit_status = EXIT_FAILURE;
		return (-1);
	}
	return (1);
}
