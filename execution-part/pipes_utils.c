/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 20:40:24 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/15 22:15:05 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_unused_pipes(int pipes[][2], int n_of_pipes, int except)
{
	int	i;

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
