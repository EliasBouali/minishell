/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:39:25 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:39:27 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*dup_n_in_string(const char *s, size_t n)
{
	char	*dst;
	size_t	i;

	dst = (char *)malloc(n + 1);
	if (!dst)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		dst[i] = s[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

static int	parse_line(const char *line, char **out_name,
		const char **out_value)
{
	const char	*equal_sign;

	equal_sign = ft_strchr(line, '=');
	*out_name = NULL;
	*out_value = NULL;
	if (!equal_sign)
	{
		*out_name = ft_strdup(line);
		if (*out_name)
			return (1);
		return (0);
	}
	*out_name = dup_n_in_string(line, (size_t)(equal_sign - line));
	*out_value = equal_sign + 1;
	if (*out_name)
		return (1);
	return (0);
}

static int	add_one_entry(t_env **env, const char *line)
{
	char		*name;
	const char	*val;
	int			status;

	if (!parse_line(line, &name, &val))
		return (2);
	if (!is_valid_name(name))
	{
		free(name);
		return (0);
	}
	status = env_set(env, name, val);
	free(name);
	if (status == 2)
		return (2);
	return (0);
}

t_env	*init_env(char **envp_src)
{
	t_env	*env;
	int		i;
	int		status;

	env = NULL;
	if (!envp_src)
		return (NULL);
	i = 0;
	while (envp_src[i])
	{
		status = add_one_entry(&env, envp_src[i]);
		if (status == 2)
		{
			free_env(&env);
			return (NULL);
		}
		i++;
	}
	return (env);
}
