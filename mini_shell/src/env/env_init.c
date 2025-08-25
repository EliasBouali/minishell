#include "../include/minishell.h"


static char	*dup_n_in_s(const char *s, size_t n)
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


//analyse une ligne d environnement au fomrat name=value ou name sans =
static int	add_one_entry(t_env **env, const char *line)
{
	const char	*eq_pos;
	char		*name;
	const char	*value_ptr;
	int			set_result;

	eq_pos = ft_strchr(line, '=');
	if (!eq_pos)
	{
		name = ft_strdup(line);
		value_ptr = NULL;
	}
	else
	{
		name = dup_n_in_s(line, (size_t)(eq_pos - line));
		value_ptr = eq_pos + 1;
	}
	if (!name)
		return (2);
	if (!is_valid_name(name))
	{
		free(name);
		return (0);
	}
	set_result = env_set(env, name, value_ptr);
	free(name);
	if (set_result == 2)
		return (2);
	return (0);
}


t_env	*init_env(char **envp_src)
{
	t_env	*env;
	int		i;
	int		value_ret;

	env = NULL;
	if (!envp_src)
		return (NULL);
	i = 0;
	while (envp_src[i])
	{
		value_ret = add_one_entry(&env, envp_src[i]);
		if (value_ret == 2)
		{
			free_env(&env);
			return (NULL);
		}
		i++;
	}
	return (env);
}
