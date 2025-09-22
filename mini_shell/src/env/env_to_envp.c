/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:40:02 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:40:03 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static size_t	env_count_with_value(t_env *env)
{
	size_t	n;

	n = 0;
	while (env)
	{
		if (env->value != NULL)
			n++;
		env = env->next;
	}
	return (n);
}

static char	*make_env_entry(const char *name, const char *value)
{
	char	*s;
	size_t	ln;
	size_t	lv;

	if (!name || !value)
		return (NULL);
	ln = ft_strlen(name);
	lv = ft_strlen(value);
	s = (char *)malloc(ln + 1 + lv + 1);
	if (!s)
		return (NULL);
	ft_memcpy(s, name, ln);
	s[ln] = '=';
	ft_memcpy(s + ln + 1, value, lv);
	s[ln + 1 + lv] = '\0';
	return (s);
}

static void	free_envp_partial(char **arr, size_t count)
{
	while (count > 0)
	{
		count--;
		free(arr[count]);
	}
	free(arr);
}

static char	**fail_env_alloc(char **arr, size_t filled)
{
	free_envp_partial(arr, filled);
	ft_putstr_fd("minishell: env alloc error\n", 2);
	return (NULL);
}

char	**env_to_envp(t_env *env)
{
	size_t	n;
	size_t	i;
	char	**arr;

	n = env_count_with_value(env);
	i = 0;
	arr = alloc_envp(n);
	if (!arr)
		return (NULL);
	while (env)
	{
		if (env->value)
		{
			arr[i] = make_env_entry(env->name, env->value);
			if (!arr[i])
				return (fail_env_alloc(arr, i));
			i++;
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}
