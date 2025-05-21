/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cases_of_expanding_plus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:11:06 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/19 19:54:26 by mdahani          ###   ########.fr       */
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

char	*case_of_normal_var_with_dquotes(char *word, int *i, char *result,
		t_env *env)
{
	int (start), (j);
	char *(key), *(value);
	(*i)++;
	start = (*i);
	while (word[*i] && (ft_isalnum(word[*i]) || word[*i] == '_'))
		(*i)++;
	key = ft_substr(word, start, (*i) - start);
	value = get_env_value(env, key);
	if (!value)
		value = ft_strdup("");
	j = 0;
	while (value[j])
	{
		result = ft_strjoin_char(result, value[j]);
		j++;
	}
	return (result);
}
