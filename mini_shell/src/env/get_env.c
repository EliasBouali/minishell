#include "../include/minishell.h"

// fonction qui va nous permetre de retrouver une paire name=valeur dans le env 
const char *env_get(t_env *env, const char *name)
{
    t_env *cur;

    if (!name)
        return (NULL);
    cur = find_by_name(env, name);
    if (!cur)
        return (NULL);
    return (cur->value);
}
