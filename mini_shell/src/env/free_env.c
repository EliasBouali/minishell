/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:40:22 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:40:23 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_env(t_env **env)
{
	t_env	*curent;
	t_env	*next;

	if (!env || !*env)
		return ;
	curent = *env;
	while (curent)
	{
		next = curent->next;
		if (curent->name)
			free(curent->name);
		if (curent->value)
			free(curent->value);
		free(curent);
		curent = next;
	}
	*env = NULL;
}
