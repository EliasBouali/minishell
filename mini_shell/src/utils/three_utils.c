#include "../include/minishell.h"

int is_valid_name(const char *s)
{
    int i;

    if (!s || s[0] == '\0')
        return 0;
    if (!((s[0] >= 'A' && s[0] <= 'Z') ||
          (s[0] >= 'a' && s[0] <= 'z') ||
          s[0] == '_'))
        return 0;
    i = 1;
    while (s[i])
    {
        if (!((s[i] >= 'A' && s[i] <= 'Z') ||
              (s[i] >= 'a' && s[i] <= 'z') ||
              (s[i] >= '0' && s[i] <= '9') ||
              s[i] == '_'))
            return 0;
        i++;
    }
    return 1;
}

t_env *find_by_name(t_env *env, const char *name)
{
    t_env   *curent;
    size_t  len;

    if (!name || !*name)
        return (NULL);
    len = ft_strlen(name);
    curent = env;
    while (curent)
    {
        if (curent->name
            && ft_strlen(curent->name) == len
            && ft_strncmp(curent->name, name, len) == 0)
            return (curent);
        curent = curent->next;
    }
    return (NULL);
}

void free_envp_array(char **arr)
{
    size_t i;

    if (!arr)
        return;
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}
void    *ft_memcpy(void *dst, const void *src, size_t n)
{
    unsigned char           *d;
    const unsigned char     *s;

    if (!dst && !src)
        return (NULL);
    d = (unsigned char *)dst;
    s = (const unsigned char *)src;
    while (n > 0)
    {
        *d = *s;
        d++;
        s++;
        n--;
    }
    return (dst);
}

long long	ft_atoll(const char *str)
{
	int			i;
	int			sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

