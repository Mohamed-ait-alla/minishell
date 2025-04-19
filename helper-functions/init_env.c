/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:35:09 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/19 19:47:46 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

t_env	*init_env(char **env)
{
	t_env	*env_list;
	t_env	*new;
	t_env	*tmp;
	char	*equal;
	int		i;
	int		end;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		new = malloc(sizeof(t_env));
		if (!new)
			return (NULL);
		equal = ft_strchr(env[i], '=');
		if (equal)
		{
			end = 0;
			while (env[i][end] && env[i][end] != '=')
				end++;
			new->key = ft_substr(env[i], 0, end);
			new->value = ft_strdup(equal + 1);
		}
		else
		{
			new->key = ft_strdup(env[i]);
			new->value = NULL;
		}
		if (env_list == NULL)
		{
			env_list = new;
			new->next = NULL;
		}
		else
		{
			tmp = env_list;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
			new->next = NULL;
		}
		i++;
	}
	return (env_list);
}
