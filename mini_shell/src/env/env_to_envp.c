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

static size_t	count_pairs(t_env *env)
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

static void	copy_str(char *dst, const char *src, size_t *j)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		dst[*j] = src[i];
		(*j)++;
		i++;
	}
}

static void	make_name_value_pair(char *dst, const char *name, const char *value)
{
	size_t	j;

	j = 0;
	copy_str(dst, name, &j);
	dst[j] = '=';
	j++;
	copy_str(dst, value, &j);
	dst[j] = '\0';
}

static char	*alloc_and_fill_entry(const char *name, const char *value)
{
	size_t	name_len;
	size_t	value_len;
	char	*buf;

	name_len = ft_strlen(name);
	value_len = ft_strlen(value);
	buf = (char *)malloc(name_len + 1 + value_len + 1);
	if (!buf)
		return (NULL);
	make_name_value_pair(buf, name, value);
	return (buf);
}

char	**env_to_envp(t_env *env)
{
	char	**arr;
	size_t	count;
	size_t	i;

	count = count_pairs(env);
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->value != NULL)
		{
			arr[i] = alloc_and_fill_entry(env->name, env->value);
			if (!arr[i])
			{
				free_envp_array(arr);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}
