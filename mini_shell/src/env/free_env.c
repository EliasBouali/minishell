#include "../include/minishell.h"

void free_env(t_env **env)
{
    t_env *curent;
    t_env *next;

    if (!env || !*env)
        return;
    curent = *env;
    while (curent)
    {
        next = curent->next;
        if (curent->name)
            free(curent->name);
        if (curent->value)
            free(curent->value);
        free(curent);
        curent = next;
    }
    *env = NULL;
}
