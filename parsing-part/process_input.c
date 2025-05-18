/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 15:11:24 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/15 15:11:52 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*process_input(char *input, int *i, t_quote_type *quote_type)
{
	char	*value;

	if (input[*i] == '|' || input[*i] == '>' || input[*i] == '<')
		value = get_operator(input, i, quote_type);
	else
		value = get_word(input, i, quote_type);
	if (!value)
		return (NULL);
	return (value);
}
