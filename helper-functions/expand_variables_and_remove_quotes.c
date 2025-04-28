/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_and_remove_quotes.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:36:51 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/28 19:55:31 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static char	*ft_strjoin_char(char *str, char c)
{
	char	*new_str;
	int		i;

	new_str = malloc(sizeof(char) * ft_strlen(str) + 2);
	if (!new_str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	free(str);
	return (new_str);
}
// get the value of the quote
static char	*get_quote_value(char *input)
{
	int		i;
	char	quote;
	int		start;
	char	*str;
	char	*before_quote;
	char	*quote_value;
	char	*after_quote;

	i = 0;
	str = ft_strdup("");
	if (input[0] == '\'' || input[0] == '"')
	{
		i++;
		quote = input[0];
		start = i;
		while (input[i] && input[i] != quote)
			i++;
		str = ft_substr(input, start, i - start);
	}
	else
	{
		before_quote = ft_strdup("");
		quote_value = ft_strdup("");
		after_quote = ft_strdup("");
		while (input[i] && input[i] != '\'' && input[i] != '"')
		{
			before_quote = ft_strjoin_char(before_quote, input[i]);
			i++;
		}
		if (input[i] == '\'' || input[i] == '"')
			quote = input[i];
		i++;
		while (input[i] && input[i] != quote)
		{
			quote_value = ft_strjoin_char(quote_value, input[i]);
			i++;
		}
		i++;
		str = ft_strjoin(before_quote, quote_value);
		while (input[i] && input[i] != quote)
		{
			after_quote = ft_strjoin_char(after_quote, input[i]);
			i++;
		}
		str = ft_strjoin(str, after_quote);
		free(before_quote);
		free(quote_value);
		free(after_quote);
	}
	return (str);
}

static char	*expand_variable_value(char *word, t_env *env)
{
	int		i;
	int		start;
	char	*result;
	char	*var_value;
	char	*tmp;
	char	*key;
	char	start_quote;
	char	end_quote;

	start_quote = '\0';
	end_quote = '\0';
	i = 0;
	result = ft_strdup("");
	while (word[i])
	{
		// [$USER'$USER'$USER]
		if (word[i] == '$' && word[i + 1] && (ft_isalpha(word[i + 1]) || word[i
				+ 1] == '_'))
		{
			if (word[i - 1] == '\'')
				start_quote = '\'';
			i++;
			start = i;
			while (word[i] && (ft_isalnum(word[i]) || word[i] == '_'))
				i++;
			if (word[i] == '\'')
				end_quote = '\'';
			if (start_quote == '\'' && end_quote == '\'')
			{
				key = ft_substr(word, start - 1, i - start + 1);
				tmp = ft_strjoin(result, key);
				free(result);
				result = tmp;
				start_quote = '\0';
				end_quote = '\0';
			}
			else
			{
				key = ft_substr(word, start, i - start);
				var_value = get_env_value(env, key);
				if (!var_value)
					var_value = ft_strdup("");
				tmp = ft_strjoin(result, var_value);
				free(result);
				result = tmp;
				free(key);
				start_quote = '\0';
				end_quote = '\0';
			}
		}
		else
		{
			result = ft_strjoin_char(result, word[i]);
			i++;
		}
	}
	return (result);
}

void	expand_variables_and_remove_quotes(t_token *tokens, t_env *env)
{
	char	*expanded_value;
	char	*quote_value;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD && ft_strchr(tokens->value, '$'))
		{
			expanded_value = expand_variable_value(tokens->value, env);
			free(tokens->value);
			tokens->value = expanded_value;
		}
		if (ft_strchr(tokens->value, '\'') || ft_strchr(tokens->value, '"'))
		{
			quote_value = get_quote_value(tokens->value);
			free(tokens->value);
			tokens->value = quote_value;
		}
		tokens = tokens->next;
	}
}

/*
static char	*ft_strjoin_char(char *str, char c)
{
	char	*new_str;
	int		i;

	new_str = malloc(sizeof(char) * ft_strlen(str) + 2);
	if (!new_str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	free(str);
	return (new_str);
}

static char	*expand_variable_value(char *word, t_env *env)
{
	int		i;
	int		start;
	char	*result;
	char	*var_value;
	char	*tmp;
	char	*key;

	i = 0;
	result = ft_strdup("");
	while (word[i])
	{
		if (word[i] == '$' && word[i + 1] && (ft_isalpha(word[i + 1]) || word[i
				+ 1] == '_' || word[i + 1] == '{'))
		{
			i++;
			if (word[i] == '{')
			{
				i++;
				start = i;
				while (word[i] && word[i] != '}')
					i++;
				key = ft_substr(word, start, i - start);
				i++;
				printf("key: %s\n", key);
				var_value = get_env_value(env, key);
				if (!var_value)
					var_value = ft_strdup("");
				tmp = ft_strjoin(result, var_value);
				free(result);
				result = tmp;
				free(key);
			}
			else
			{
				start = i;
				while (word[i] && (ft_isalnum(word[i]) || word[i] == '_'))
					i++;
				key = ft_substr(word, start, i - start);
				var_value = get_env_value(env, key);
				if (!var_value)
					var_value = ft_strdup("");
				tmp = ft_strjoin(result, var_value);
				free(result);
				result = tmp;
				free(key);
			}
		}
		else
		{
			result = ft_strjoin_char(result, word[i]);
			i++;
		}
	}
	return (result);
}

// expand the env
void	expand_variables(t_token *tokens, t_env *env)
{
	char	*expanded_value;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD && ft_strchr(tokens->value, '$'))
		{
			if (tokens->quote_type != SINGLE_QUOTE)
			{
				expanded_value = expand_variable_value(tokens->value, env);
				if (tokens->value)
					free(tokens->value);
				tokens->value = expanded_value;
			}
		}
		tokens = tokens->next;
	}
}
*/