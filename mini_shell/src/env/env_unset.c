/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:40:14 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:40:16 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	remove_head(t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	*env = (*env)->next;
	free(tmp->name);
	if (tmp->value)
		free(tmp->value);
	free(tmp);
	return (0);
}

static int	remove_next(t_env *prev)
{
	t_env	*current;

	current = prev->next;
	prev->next = current->next;
	free(current->name);
	if (current->value)
		free(current->value);
	free(current);
	return (0);
}

int	env_unset(t_env **env, const char *name)
{
	t_env	*preview;
	t_env	*current;

	if (!env || !is_valid_name(name))
		return (1);
	if (!*env)
		return (0);
	if (ft_strlen((*env)->name) == ft_strlen(name) && ft_strncmp((*env)->name,
			name, ft_strlen(name)) == 0)
		return (remove_head(env));
	preview = *env;
	current = preview->next;
	while (current)
	{
		if (ft_strlen(current->name) == ft_strlen(name)
			&& ft_strncmp(current->name, name, ft_strlen(name)) == 0)
			return (remove_next(preview));
		preview = current;
		current = current->next;
	}
	return (0);
}
