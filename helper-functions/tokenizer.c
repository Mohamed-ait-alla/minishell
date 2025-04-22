/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:09:16 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/22 12:55:20 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

// advanced get_quote_value function
// static char	*get_quote_value(char **input, int *i, t_quote_type *quote_type)
// {
// 	char	quote;
// 	int		start;
// 	char	*str;
// 	char	*line;
// 	char	*tmp;
// 	char	*new_input;

// 	quote = (*input)[*i];
// 	(*i)++;
// 	start = (*i);
// 	while ((*input)[*i] && (*input)[*i] != quote)
// 		(*i)++;
// 	while ((*input)[*i] == '\0')
// 	{
// 		if (quote == '\'')
// 			line = readline("quote> ");
// 		else
// 			line = readline("dquote> ");
// 		if (!line)
// 			break ;
// 		tmp = ft_strjoin((*input), "\n");
// 		new_input = ft_strjoin(tmp, line);
// 		free(tmp);
// 		free((*input));
// 		(*input) = new_input;
// 		free(line);
// 		(*i) = start;
// 		while ((*input)[*i] && (*input)[*i] != quote)
// 			(*i)++;
// 	}
// 	str = ft_substr((*input), start, *i - start);
// 	if ((*input)[*i] == quote)
// 		(*i)++;
// 	if (quote == '\'')
// 		*quote_type = SINGLE_QUOTE;
// 	else
// 		*quote_type = DOUBLE_QUOTE;
// 	return (str);
// }

static char	*get_quote_value(char *input, int *i, t_quote_type *quote_type)
{
	char	quote;
	int		start;
	char	*str;

	quote = input[*i];
	(*i)++;
	start = (*i);
	while (input[*i] && input[*i] != quote)
		(*i)++;
	str = ft_substr(input, start, *i - start);
	if (input[*i] == quote)
		(*i)++;
	if (quote == '\'')
	    *quote_type = SINGLE_QUOTE;
	else
		*quote_type = DOUBLE_QUOTE;
	return (str);
}

static char	*get_operator(char *input, int *i)
{
	char	*operator;

	if (!ft_strncmp(input + *i, ">>", 2) || !ft_strncmp(input + *i, "<<", 2))
	{
		operator= ft_substr(input, *i, 2);
		*i += 2;
	}
	else
	{
		operator= ft_substr(input, *i, 1);
		*i += 1;
	}
	return (operator);
}

static char	*get_word(char *input, int *i)
{
	int	start;

	start = *i;
	while (input[*i] && (input[*i] != '|' && input[*i] != '>'
			&& input[*i] != '<') && input[*i] > 32)
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

static t_token_type	get_token_type(char *value)
{
	if (!ft_strncmp(value, ">>", 2))
		return (TOKEN_APPEND);
	else if (!ft_strncmp(value, "<<", 2))
		return (TOKEN_HEREDOC);
	else if (!ft_strncmp(value, "|", 1))
		return (TOKEN_PIPE);
	else if (!ft_strncmp(value, ">", 1))
		return (TOKEN_REDIRECT_OUT);
	else if (!ft_strncmp(value, "<", 1))
		return (TOKEN_REDIRECT_IN);
	return (TOKEN_WORD);
}

static t_token	*new_token(char *value, t_token_type type,
		t_quote_type quote_type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = value;
	new_token->type = type;
	new_token->quote_type = quote_type;
	new_token->next = NULL;
	return (new_token);
}

static void	add_token(t_token **head, t_token *new_token)
{
	t_token	*tmp;

	if (!*head)
		*head = new_token;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}

// split the cmd to tokens
t_token	*tokenize_input(char *input)
{
	t_token			*tokens;
	int				i;
	char			*value;
	t_token_type	type;
	t_quote_type	quote_type;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] <= 32)
			i++;
		if (input[i] == '\0')
			break ;
		quote_type = NO_QUOTE;
		// handle the single and double quote and get the value of quotes
		if (input[i] == '\'' || input[i] == '"')
			value = get_quote_value(input, &i, &quote_type);
			// advanced get_quote_value function
			// value = get_quote_value(&input, &i, &quote_type);
		// get the opearator
		else if (input[i] == '|' || input[i] == '>' || input[i] == '<')
			value = get_operator(input, &i);
		// get the word
		else
			value = get_word(input, &i);
		// add a type of token (PIPE, WORD, ...)
		type = get_token_type(value);
		// add a new token to list of tokens
		add_token(&tokens, new_token(value, type, quote_type));
	}
	return (tokens);
}
