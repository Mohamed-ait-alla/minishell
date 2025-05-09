/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 10:22:51 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/09 10:17:52 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_get_len(const char *s, char c)
{
	size_t	len;
	size_t	i;

	len = 0;
	i = 0;
	while (s && s[i])
	{
		while (s[i] && (s[i] == c))
			i++;
		if (s[i])
			len++;
		while (s[i] && (s[i] != c))
			i++;
	}
	return (len);
}

static	int	ft_get_len_word(const char *str, char c)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (str[i] && (str[i] != c))
	{
		len++;
		i++;
	}
	return (len);
}

void	ft_free_all(char **strings, int i)
{
	while (i--)
		free(strings[i]);
	free(strings);
}

static char	*ft_get_word(const char *str, char c)
{
	char	*word;
	int		i;
	int		len_word;

	len_word = ft_get_len_word(str, c);
	word = (char *)ft_malloc((len_word + 1) * sizeof(char), 1);
	if (!word)
	{
		return (NULL);
	}
	i = 0;
	while (i < len_word)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**strings;
	int		i;

	strings = (char **)ft_malloc((ft_get_len(s, c) + 1) * sizeof(char *), 1);
	if (!strings)
		return (NULL);
	i = 0;
	while (s && *s)
	{
		while (*s && (*s == c))
			s++;
		if (*s)
		{
			strings[i++] = ft_get_word(s, c);
			if (strings[i - 1] == NULL)
			{
				ft_free_all(strings, i - 1);
				return (NULL);
			}
		}
		while (*s && (*s != c))
			s++;
	}
	strings[i] = 0;
	return (strings);
}
