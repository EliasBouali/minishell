#include "../include/minishell.h"

int env_set(t_env **env, const char *name, const char *value)
{
    t_env   *curent;
    char    *copy;

    if (!env || !name || !is_valid_name(name))
        return (1);

    curent = find_by_name(*env, name);
    if (curent)
    {
        if (value == NULL)
        {
            if (curent->value)
            {
                free(curent->value);
                curent->value = NULL;
            }
            return (0);
        }
        copy = ft_strdup(value);
        if (!copy)
            return (2);
        if (curent->value)
            free(curent->value);
        curent->value = copy;
        return (0);
    }
    curent = env_node_new(name, value);
    if (!curent)
        return (2);
    if (*env == NULL)
        *env = curent;
    else
    {
        t_env *last_node = *env;
        while (last_node->next)
            last_node = last_node->next;
        last_node->next = curent;
    }
    return (0);
}
