/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:54:45 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/06 13:32:11 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../includes/minishell.h"

char	*ft_strdup(const char *s1)
{
	char	*new;
	size_t	size;
	size_t	i;

	i = 0;
	size = ft_strlen(s1);
	new = (char *)ft_malloc(sizeof(char) * (size + 1), 1);
	if (!new)
		return (NULL);
	while (i < size)
	{
		new[i] = s1[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}
