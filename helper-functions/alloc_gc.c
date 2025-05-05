/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_gc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:37:52 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/05 17:26:35 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	*alloc_gc(size_t size, t_gc **gc_list)
{
	t_gc	*new_node;
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);

	new_node = malloc(sizeof(t_gc));
	if (!new_node)
	{
		free(ptr);
		return (NULL);
	}
	new_node->ptr = ptr;
	new_node->next = *gc_list;
	*gc_list = new_node;

	return (ptr);
}
