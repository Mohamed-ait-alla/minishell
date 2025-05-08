/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 19:01:57 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/08 20:11:15 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current_node;
	t_list	*next_node;

	if (!lst || !del || !(*lst))
		return ;
	current_node = *lst;
	while (current_node)
	{
		next_node = current_node->next;
		if (current_node->content)
			del(current_node->content);
		free(current_node);
		current_node = next_node;
	}
	*lst = NULL;
}
