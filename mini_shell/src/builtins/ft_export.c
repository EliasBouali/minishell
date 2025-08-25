#include "../../include/minishell.h"


static int print_env_from_tenv(t_env *env)
{
    t_env *cur;

    cur = env;
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


static int split_name_value(const char *arg, char **name_out, char **value_out)
{
    size_t  i;
    size_t  j;
    char    *name;
    char    *value;

    if (!arg || !name_out || !value_out)
        return (0);
    i = 0;
    while (arg[i] && arg[i] != '=')
        i++;
    if (arg[i] == '=')
    {
        name = (char *)malloc(i + 1);
        if (!name)
            return (0);
        j = 0;
        while (j < i)
        {
            name[j] = arg[j];
            j++;
        }
        name[i] = '\0';
        value = ft_strdup(arg + i + 1);
        if (!value)
        {
            free(name);
            return (0);
        }
        *name_out = name;
        *value_out = value;
        return (1);
    }
    name = ft_strdup(arg);
    if (!name)
        return (0);
    *name_out = name;
    *value_out = NULL;
    return (1);
}

static void print_export_invalid(const char *arg)
{
    ft_putstr_fd("minishell: export: `", 2);
    ft_putstr_fd((char *)arg, 2);
    ft_putstr_fd("': not a valid identifier\n", 2);
}

int ft_export(char **argv, t_env **env)
{
    int   i;
    int   exit_code;
    char *name;
    char *value;
    int   rc;

    if (!env)
        return (1);
    if (!argv || !argv[1])
        return print_env_from_tenv(*env);
    exit_code = 0;
    i = 1;
    while (argv[i])
    {
        if (!split_name_value(argv[i], &name, &value))
        {
            ft_putstr_fd("minishell: export: allocation error\n", 2);
            exit_code = 1;
            i++;
            continue;
        }
        if (!is_valid_name((const char *)name))
        {
            print_export_invalid(argv[i]);
            exit_code = 1;
        }
        else
        {
            rc = env_set(env, (const char *)name, (const char *)value);
            if (rc == 2)
            {
                ft_putstr_fd("minishell: export: allocation error\n", 2);
                exit_code = 1;
            }
        }
        if (name)
            free(name);
        if (value)
            free(value);
        i++;
    }
    return (exit_code);
}
