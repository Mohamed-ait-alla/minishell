/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 20:26:14 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/19 20:30:12 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (!ft_strncmp(env->key, key, ft_strlen(key)))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}
