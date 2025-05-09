/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:33:53 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/09 18:06:58 by mait-all         ###   ########.fr       */
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
		exit (exit_code);
	}
	return (exit_code);
}

void	manage_shell_errors(char *arg, char *path)
{
	if (!arg)
	{
		ft_malloc(0, 0);
		exit (EXIT_FAILURE);
	}
	if (!path)
		custom_error(ERR_CMD_NOT_FOUND, arg, 127, false);
	if (ft_strncmp(path, "no file", ft_strlen(path)) == 0)
		custom_error(ERR_NO_FILE, arg, 127, false);
}
