/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:39:46 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:39:48 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	update_existing(t_env *node, const char *value)
{
	char	*copy;

	if (value == NULL)
	{
		if (node->value)
			free(node->value);
		node->value = NULL;
		return (0);
	}
	copy = ft_strdup(value);
	if (!copy)
		return (2);
	if (node->value)
		free(node->value);
	node->value = copy;
	return (0);
}

static int	append_new(t_env **env, const char *name, const char *value)
{
	t_env	*node;
	t_env	*last;

	node = env_node_new(name, value);
	if (!node)
		return (2);
	if (!*env)
		*env = node;
	else
	{
		last = *env;
		while (last->next)
			last = last->next;
		last->next = node;
	}
	return (0);
}

int	env_set(t_env **env, const char *name, const char *value)
{
	t_env	*current;

	if (!env || !name || !is_valid_name(name))
		return (1);
	current = find_by_name(*env, name);
	if (current)
		return (update_existing(current, value));
	return (append_new(env, name, value));
}
