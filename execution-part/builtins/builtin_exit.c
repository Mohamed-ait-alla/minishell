/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:39:53 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/20 20:27:39 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_numeric(char *arg)
{
	int	i;

	i = 0;
	if (arg)
	{
		if ((arg[0] == '+' && !ft_isdigit(arg[1]))
			|| (arg[0] == '-' && !ft_isdigit(arg[1])))
			return (0);
		i = 1;
	}
	while (arg && arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char	**args, int last_cmd_exit_status)
{
	if (!args[1])
		return (last_cmd_exit_status);
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("bash: exit: numeric argument required\n", STDERR_FILENO);
		return (2);
	}
	if (args[2])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return ((unsigned char)ft_atoi(args[1]));
}
