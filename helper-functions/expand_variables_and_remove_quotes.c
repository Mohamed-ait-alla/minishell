/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_and_remove_quotes.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:36:51 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/15 11:31:37 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static char	*ft_strjoin_char(char *str, char c)
{
	char	*new_str;
	int		i;

	new_str = ft_malloc(sizeof(char) * (ft_strlen(str) + 2), 1);
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
	return (new_str);
}

static char	*expand_variable_value(char *word, t_env *env)
{
	int		i;
	char	*result;
	char	*key;
	char	*value;
	char	*tmp;
	int		start;

	i = 0;
	result = ft_strdup("");
	while (word[i])
	{
		if (word[i] == '\'')
		{
			i++;
			while (word[i] && word[i] != '\'')
			{
				result = ft_strjoin_char(result, word[i]);
				i++;
			}
			if (word[i])
				i++;
		}
		else if (word[i] == '$' && word[i + 1] && (ft_isalpha(word[i + 1])
				|| word[i + 1] == '_'))
		{
			i++;
			start = i;
			while (word[i] && (ft_isalnum(word[i]) || word[i] == '_'))
				i++;
			key = ft_substr(word, start, i - start);
			value = get_env_value(env, key);
			if (!value)
				value = ft_strdup("");
			int j = 0;
			while (value[j])
			{
				while (value[j] && value[j] <= 32 && value[j + 1] <= 32)
					j++;
				if (value[j])
				{
					result = ft_strjoin_char(result, value[j]);
					j++;
				}
			}
			// tmp = ft_strjoin(result, value);
			// free(result);
			// result = tmp;
			// free(key);
		}
		else if (word[i] == '"')
		{
			i++;
			while (word[i] && word[i] != '"')
			{
				if (word[i] == '$' && word[i + 1] && (ft_isalnum(word[i + 1])
						|| word[i + 1] == '_'))
				{
					i++;
					start = i;
					while (word[i] && (ft_isalnum(word[i]) || word[i] == '_'))
						i++;
					key = ft_substr(word, start, i - start);
					value = get_env_value(env, key);
					if (!value)
						value = ft_strdup("");
					tmp = ft_strjoin(result, value);
					// free(result);
					result = tmp;
					// free(key);
				}
				else if (word[i] == '$' && word[i + 1] && word[i + 1] == '?')
				{
					i += 2;
					// resplace the 0 with the last return value
					result = ft_strjoin(result, ft_itoa(g_exit_status));
				}
				else
				{
					result = ft_strjoin_char(result, word[i]);
					i++;
				}
			}
			if (word[i])
				i++;
		}
		else if (word[i + 1] && word[i] == '$' && word[i + 1] == '\'')
		{
			i++;
			while (word[i] && word[i] != '\'')
			{
				result = ft_strjoin_char(result, word[i]);
				i++;
			}
			if (word[i])
				i++;
		}
		else if (word[i + 1] && word[i] == '$' && word[i + 1] == '\"')
		{
			i++;
			while (word[i] && word[i] != '\"')
			{
				result = ft_strjoin_char(result, word[i]);
				i++;
			}
			if (word[i])
				i++;
		}
		else if (word[i] == '$' && word[i + 1] && ft_isdigit(word[i + 1]))
		{
			i+= 2;
			while (word[i])
			{
				result = ft_strjoin_char(result, word[i]);
				i++;
			}
			if (word[i])
				i++;
		}
		else if (word[i] == '$' && word[i + 1] && word[i + 1] == '?')
		{
			i += 2;
			// resplace the 0 with the last return value
			result = ft_strjoin(result, ft_itoa(g_exit_status));
		}
		else
		{
			result = ft_strjoin_char(result, word[i]);
			i++;
		}
	}
	return (result);
}

// expand the env variables and remove quotes
void	expand_variables_and_remove_quotes(t_token *tokens, t_env *env)
{
	char	*expanded_value;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			expanded_value = expand_variable_value(tokens->value, env);
			// free(tokens->value);
			tokens->value = expanded_value;
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