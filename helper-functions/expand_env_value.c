/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_value.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:36:51 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/20 19:22:33 by mdahani          ###   ########.fr       */
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

char	*expand_variable_value(char *word, t_env *env)
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
				+ 1] == '_'))
		{
			i++;
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
		else
		{
			result = ft_strjoin_char(result, word[i]);
			i++;
		}
	}
	return (result);
}
