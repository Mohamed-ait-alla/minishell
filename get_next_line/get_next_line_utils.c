/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 09:51:18 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/06 13:25:10 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char				*new;
	unsigned int		i;
	unsigned int		j;

	if (!s1 || !s2)
		return (NULL);
	new = malloc((ft_gnlstrlen(s1) + ft_gnlstrlen(s2) + 1) * sizeof(char));
	if (!new)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		new[i] = s2[j];
		i++;
		j++;
	}
	new[i] = '\0';
	return (new);
}

size_t	ft_gnlstrlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s && s[len])
		len++;
	return (len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	if (!s)
		return (NULL);
	if (ft_gnlstrlen(s) < start)
		return (ft_strdup(""));
	if (len > (ft_gnlstrlen(s) - start))
		len = ft_gnlstrlen(s) - start;
	sub = (char *)malloc((len + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len && start < ft_gnlstrlen(s))
	{
		*(sub + i) = *(s + start);
		start++;
		i++;
	}
	*(sub + i) = '\0';
	return (sub);
}

char	*ft_strdup(const char *s1)
{
	char	*new;
	size_t	size;
	size_t	i;

	i = 0;
	size = ft_gnlstrlen(s1);
	new = (char *)malloc(sizeof(char) * (size + 1));
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
