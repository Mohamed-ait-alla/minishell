/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:40:24 by mait-all          #+#    #+#             */
/*   Updated: 2025/04/12 11:08:24 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

bool	check_for_pipes(char **av)
{
	int i;

	i = 0;
	while (av[i])
	{
		if (av[i][0] == '|')
			return (true);
		i++;
	}
	return (false);
}

int	calculate_number_of_pipes(char **av)
{
	int i;
	int counter;

	i = 0;
	counter = 0;
	while (av[i])
	{
		if (av[i][0] == '|')
			counter++;
		i++;
	}
	return (counter);
}

void	close_unused_pipes(int pipes[][2], int n_of_pipes, int except)
{
	int i;

	i = 0;
	while (i < n_of_pipes)
	{
		if (i != except)
		{
			close (pipes[i][0]);
			close (pipes[i][1]);
		}
		i++;
	}
}
