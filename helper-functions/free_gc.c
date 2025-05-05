/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_gc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:44:18 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/05 16:22:22 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_gc(t_gc **collector)
{
	t_gc	*tmp;

	while (*collector)
	{
		tmp = (*collector)->next;
		free((*collector)->ptr);
		free(*collector);
		*collector = tmp;
	}
}
