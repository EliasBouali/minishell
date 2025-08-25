#include "../include/minishell.h"

int env_unset(t_env **env, const char *name)
{
    t_env *cur;
    t_env *prev;

    if (!env || !is_valid_name(name))
        return (1);
    if (!*env)
        return (0);
    if (ft_strncmp((*env)->name, name, ft_strlen(name)) == 0
        && ft_strlen((*env)->name) == ft_strlen(name))
    {
        cur = *env;
        *env = (*env)->next;
        free(cur->name);
        if (cur->value)
            free(cur->value);
        free(cur);
        return (0);
    }
    prev = *env;
    cur = prev->next;
    while (cur)
    {
        if (ft_strlen(cur->name) == ft_strlen(name)
            && ft_strncmp(cur->name, name, ft_strlen(name)) == 0)
        {
            prev->next = cur->next;
            free(cur->name);
            if (cur->value)
                free(cur->value);
            free(cur);
            return (0);
        }
        prev = cur;
        cur = cur->next;
    }
    return (0);
}
