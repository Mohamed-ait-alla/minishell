/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 12:50:05 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/09 18:08:16 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// func to add element to array and return the array
char	**ft_realloc_array(char **arr, char *new_str)
{
	int		len;
	int		i;
	char	**new_arr;

	len = 0;
	if (arr)
		while (arr[len])
			len++;
	new_arr = ft_malloc(sizeof(char *) * (len + 2), 1);
	if (!new_arr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[len] = ft_strdup(new_str);
	new_arr[len + 1] = NULL;
	return (new_arr);
}
