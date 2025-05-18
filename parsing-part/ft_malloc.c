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

void	*ft_malloc(size_t size, int mode)
{
	static t_list	*head_list;
	t_list			*new_node;
	void			*content;

	new_node = NULL;
	content = NULL;
	if (mode == 1)
	{
		content = malloc(size);
		if (!content)
			return (NULL);
		new_node = ft_lstnew(content);
		ft_lstadd_back(&head_list, new_node);
	}
	else if (mode == 0)
	{
		ft_lstclear(&head_list, free);
		head_list = NULL;
	}
	return (content);
}
