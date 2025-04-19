/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdahani <mdahani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:04:47 by mdahani           #+#    #+#             */
/*   Updated: 2025/04/19 18:47:33 by mdahani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int	main(int ac, char **av, char **env)
{
	int		i;
	char	*env_var;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	printf("-------------------------------------\n");
	env_var = getenv("PATH");
	if (env_var)
		printf("PATH: %s\n", env_var);
	else
		printf("PATH not found\n");
}
