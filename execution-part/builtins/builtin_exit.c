/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:39:53 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/25 19:50:52 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/minishell.h"

static int	is_numeric(char *arg)
{
	int	i;

	i = 0;
	while (arg && arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	builtin_exit(char	**args)
{
	if (!args[1])
		exit(EXIT_SUCCESS); // exit with the last cmd's status
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("bash: exit: numeric argument required\n", STDERR_FILENO);
		exit(255);
	}
	if (args[2])
	{
		ft_putstr_fd("bash: exit: too many arguments\n", STDERR_FILENO);
		return ;
	}
	exit (ft_atoi(args[1]));
}