/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:09:16 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/07 10:53:50 by mdahani          ###   ########.fr       */
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


// static char	*get_quote_value(char *input, int *i)
// {
// 	char	quote;
// 	int		start;
// 	char	*str;

// 	quote = input[*i];
// 	(*i)++;
// 	start = (*i);
// 	while (input[*i] && input[*i] != quote)
// 		(*i)++;
// 	str = ft_substr(input, start, *i - start);
// 	if (input[*i] == quote)
// 		(*i)++;
// 	else
// 	{
// 		free(str);
// 		return (NULL);
// 	}
// 	return (str);
// }

static char	*get_operator(char *input, int *i, t_quote_type *quote_type)
{
	char	*operator;

	if (!ft_strncmp(input + *i, ">>", 2) || !ft_strncmp(input + *i, "<<", 2))
	{
		operator= ft_substr(input, *i, 2);
		*i += 2;
		if (input[*i] == '>' || input[*i] == '<')
			return (NULL);
	}
	else
	{
		operator= ft_substr(input, *i, 1);
		*i += 1;
		if (input[*i] == '|')
			return (NULL);
	}
	if (input[*i] == '\'')
			*quote_type = SINGLE_QUOTE;
	else if (input[*i] == '"')
			*quote_type = DOUBLE_QUOTE;
	return (operator);
}

static char	*get_word(char *input, int *i, t_quote_type *quote_type)
{
	int	start;
	char quote;

	start = *i;
	while (input[*i] && (input[*i] != '|' && input[*i] != '>'
			&& input[*i] != '<') && input[*i] > 32)
	{
		if (input[*i] == '"' || input[*i] == '\'')
		{
			quote = input[*i];
			(*i)++;
			while (input[*i] && input[*i] != quote)
				(*i)++;
			if (input[*i] != quote)
				return (NULL);
			if (quote == '\'')
				*quote_type = SINGLE_QUOTE;
			else if (quote == '"') 
				*quote_type = DOUBLE_QUOTE;
		}
		(*i)++;
	}
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

static t_token	*new_token(char *value, t_token_type type, t_quote_type quote_type)
{
	t_token	*new_token;

	new_token = ft_malloc(sizeof(t_token), 1);
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
	quote_type = NO_QUOTE;
	while (input[i])
	{
		while (input[i] && input[i] <= 32)
			i++;
		if (input[i] == '\0')
			break ;
		// handle the single and double quote and get the value of quotes
		// if (input[i] == '\'' || input[i] == '"')
		// {
		// 	value = get_quote_value(input, &i);
		// 	// advanced get_quote_value function
		// 	// value = get_quote_value(&input, &i, &quote_type);
		// 	if (!value)
		// 	{
		// 		tokens = NULL;
		// 		break ;
		// 	}
		// }
		// get the opearator
		if (input[i] == '|' || input[i] == '>' || input[i] == '<')
		{
			value = get_operator(input, &i, &quote_type);
			if (!value)
			{
				tokens = NULL;
				break ;	
			}
		}
		// get the word
		else
		{
			value = get_word(input, &i, &quote_type);
			if (!value)
			{
				tokens = NULL;
				break ;
			}
		}
		// add a type of token (PIPE, WORD, ...)
		if (value[0] > 32 && ft_strlen(value) >= 1)
		{
			type = get_token_type(value);
			// add a new token to list of tokens
			add_token(&tokens, new_token(value, type, quote_type));
		}
	}
	return (tokens);
}
