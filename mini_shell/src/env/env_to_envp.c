#include "../include/minishell.h"

static int	make_name_value_pair(char *dst, const char *name, const char *value)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (name[i])
	{
		dst[j] = name[i];
		i++;
		j++;
	}
	dst[j] = '=';
	j++;
	i = 0;
	while (value[i])
	{
		dst[j] = value[i];
		i++;
		j++;
	}
	dst[j] = '\0';
	return (0);
}

char	**env_to_envp(t_env *env)
{
	size_t	count;
	t_env	*cur;
	char	**arr;
	size_t	i;
	size_t	name_len;
	size_t	value_len;
	char	*buf;

	count = 0;
	cur = env;
	while (cur)
	{
		if (cur->value != NULL)
			count++;
		cur = cur->next;
	}
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	i = 0;
	cur = env;
	while (cur)
	{
		if (cur->value != NULL)
		{
			name_len = ft_strlen(cur->name);
			value_len = ft_strlen(cur->value);
			buf = (char *)malloc(name_len + 1 + value_len + 1);
			if (!buf)
			{
				free_envp_array(arr);
				return (NULL);
			}
			make_name_value_pair(buf, cur->name, cur->value);
			arr[i] = buf;
			i++;
		}
		cur = cur->next;
	}
	arr[i] = NULL;
	return (arr);
}
