/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 22:48:53 by mait-all          #+#    #+#             */
/*   Updated: 2024/11/04 18:16:41 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	len_src;
	size_t	len_dst;
	size_t	i;
	size_t	j;

	len_src = ft_strlen(src);
	if (dstsize == 0 && dst == NULL)
		return (len_src);
	len_dst = ft_strlen(dst);
	if (len_dst >= dstsize)
		return (len_src + dstsize);
	j = len_dst;
	i = 0;
	while (src[i] && i < dstsize - len_dst - 1)
	{
		dst[j] = src[i];
		i++;
		j++;
	}
	if (j < dstsize)
		dst[j] = '\0';
	return (len_src + len_dst);
}
