/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:41:29 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/15 20:28:36 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	set_echo_flags(int *i, int *is_option, int *offset)
{
	*i = 2;
	*is_option = 1;
	*offset = 2;
}

int	builtin_echo(char **args, char **env)
{
	int	i;
	int	offset;
	int	is_option;

	i = 1;
	is_option = 0;
	offset = 1;
	if (!args[1])
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (EXIT_SUCCESS);
	}
	if (ft_strncmp(args[1], "-n", 2) == 0)
		set_echo_flags(&i, &is_option, &offset);
	while (args[i])
	{
		if (i != offset)
			ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putstr_fd(args[i], STDOUT_FILENO);
		i++;
	}
	if (!is_option)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
