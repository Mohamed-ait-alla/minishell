/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cases_of_expanding_plus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:11:06 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/18 13:49:21 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*case_of_var_with_next_char_digit(char *word, int *i, char *result)
{
	(*i) += 2;
	while (word[*i])
	{
		result = ft_strjoin_char(result, word[*i]);
		(*i)++;
	}
	if (word[*i])
		(*i)++;
	return (result);
}

char	*case_of_var_with_exit_status(int *i, char *result)
{
	(*i) += 2;
	result = ft_strjoin(result, ft_itoa(g_exit_status));
	return (result);
}

char	*case_of_word(char *word, int *i, char *result)
{
	result = ft_strjoin_char(result, word[*i]);
	(*i)++;
	return (result);
}
