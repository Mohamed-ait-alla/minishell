/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:33:53 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/06 14:27:59 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	custom_error(char *err_msg, char *arg, int exit_code)
{
	ft_putstr_fd(err_msg, STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit (exit_code);
}

void	manage_shell_errors(char *arg, char *path)
{
	if (!arg)
		exit (EXIT_FAILURE);
	if (!path)
		custom_error(ERR_CMD_NOT_FOUND, arg, 127);
	if (ft_strncmp(path, "no file", ft_strlen(path)) == 0)
		custom_error(ERR_NO_FILE, arg, 127);
}
