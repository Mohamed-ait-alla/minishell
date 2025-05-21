/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables_and_remove_quotes.c               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:36:51 by mdahani           #+#    #+#             */
/*   Updated: 2025/05/19 17:25:39 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

static char	*get_limiter(char *limiter)
{
	int		i;
	char	quote;
	char	*result;

	i = 0;
	result = ft_strdup("");
	while (limiter && limiter[i])
	{
		if (limiter[i] == '"' || limiter[i] == '\'')
		{
			quote = limiter[i];
			i++;
			while (limiter[i] && limiter[i] != quote)
			{
				result = ft_strjoin_char(result, limiter[i]);
				i++;
			}
			i++;
		}
		else
			result = ft_strjoin_char(result, limiter[i++]);
	}
	return (result);
}

static char	*process_of_expanding(char *word, int *i, char *result, t_env *env)
{
	if (word[*i] == '\'')
		result = case_of_squote(word, i, result);
	else if (word[*i] == '$' && word[*i + 1] && (ft_isalpha(word[*i + 1])
			|| word[*i + 1] == '_'))
		result = case_of_normal_var(word, i, result, env);
	else if (word[*i] == '"')
		result = case_of_dquote(word, i, result, env);
	else if (word[*i + 1] && word[*i] == '$' && word[*i + 1] == '\'')
		result = case_of_var_with_next_char_squote(word, i, result);
	else if (word[*i + 1] && word[*i] == '$' && word[*i + 1] == '\"')
		result = case_of_var_with_next_char_dquote(word, i, result);
	else if (word[*i] == '$' && word[*i + 1] && ft_isdigit(word[*i + 1]))
		result = case_of_var_with_next_char_digit(word, i, result);
	else if (word[*i] == '$' && word[*i + 1] && word[*i + 1] == '?')
		result = case_of_var_with_exit_status(i, result);
	else
		result = case_of_word(word, i, result);
	return (result);
}

static char	*expand_variable_value(char *word, t_env *env, int *is_here_doc)
{
	int		i;
	char	*result;

	i = 0;
	if (*is_here_doc)
	{
		result = get_limiter(word);
		*is_here_doc = 0;
		return (result);
	}
	result = ft_strdup("");
	while (word[i])
		result = process_of_expanding(word, &i, result, env);
	return (result);
}

void	expand_variables_and_remove_quotes(t_token *tokens, t_env *env)
{
	char	*expanded_value;
	int		is_here_doc;

	is_here_doc = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_HEREDOC)
			is_here_doc = 1;
		if (tokens->type == TOKEN_WORD)
		{
			expanded_value = expand_variable_value(tokens->value, env,
					&is_here_doc);
			tokens->value = expanded_value;
		}
		tokens = tokens->next;
	}
}
