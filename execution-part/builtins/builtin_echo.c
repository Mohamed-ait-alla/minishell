/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:41:29 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/19 20:13:03 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	has_only_n(char *arg)
{
	int	i;

	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	builtin_echo(char **args)
{
	int (n_flag), (i), (j);
	n_flag = 0;
	i = 1;
	j = 0;
	while (args[i] && ft_strncmp(args[i], "-n", 2) == 0)
	{
		if (has_only_n(args[i]))
			n_flag = 1;
		else
			break ;
		i++;
	}
	while (args[i])
	{
		if (j != 0)
			printf(" ");
		printf("%s", args[i]);
		j++;
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (EXIT_SUCCESS);
}
