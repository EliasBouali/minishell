/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:40:30 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:40:31 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// fonction qui va nous permetre de retrouver une paire name=valeur dans le env
const char	*env_get(t_env *env, const char *name)
{
	t_env	*cur;

	if (!name)
		return (NULL);
	cur = find_by_name(env, name);
	if (!cur)
		return (NULL);
	return (cur->value);
}
