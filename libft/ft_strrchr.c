/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 12:57:40 by mait-all          #+#    #+#             */
/*   Updated: 2024/10/29 20:37:36 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;

	len = ft_strlen(s);
	while (len > 0)
	{
		if (*(s + len - 1) == (char)c)
			return ((char *)s + len - 1);
		len--;
	}
	if ((char) c == '\0')
		return ((char *)s + ft_strlen(s));
	return (NULL);
}
