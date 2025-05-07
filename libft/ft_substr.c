/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 21:57:32 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/06 13:27:18 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../includes/minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	if (!s)
		return (NULL);
	if (ft_strlen(s) <= start)
		return (ft_strdup(""));
	if (len > (ft_strlen(s) - start))
		len = ft_strlen(s) - start;
	sub = (char *)ft_malloc((len + 1) * sizeof(char), 1);
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len && start < ft_strlen(s))
	{
		*(sub + i) = *(s + start);
		start++;
		i++;
	}
	*(sub + i) = '\0';
	return (sub);
}
