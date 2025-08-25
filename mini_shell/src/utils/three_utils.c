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
