/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_gc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:44:18 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/05 17:41:42 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_gc(t_gc **gc_list)
{
	t_gc	*tmp;

	while (*gc_list)
	{
		tmp = *gc_list;
		if (tmp->ptr)
			free(tmp->ptr);
		*gc_list = (*gc_list)->next;
		free(tmp);
	}
	*gc_list = NULL;
}
