#include "../../include/minishell.h"


int ft_env(char **argv, t_env **env)
{
    t_env *cur;

    (void)argv;
    if (!env || !*env)
        return (0);
    cur = *env;
    while (cur)
    {
        if (cur->value != NULL)
        {
            ft_putstr_fd(cur->name, 1);
            ft_putstr_fd("=", 1);
            ft_putstr_fd(cur->value, 1);
            ft_putstr_fd("\n", 1);
        }
        cur = cur->next;
    }
    return (0);
}
