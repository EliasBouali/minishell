#include "../../include/minishell.h"


static void print_cd_error(const char *path)
{
    ft_putstr_fd("minishell: cd: ", 2);
    ft_putstr_fd((char *)path, 2);
    ft_putstr_fd(": ", 2);
    ft_putstr_fd((char *)strerror(errno), 2);
    ft_putstr_fd("\n", 2);
}

int ft_cd(char **argv, t_env **env)
{
    const char  *target;
    char        *oldpwd;
    char        *newpwd;

    if (!env)
        return (1);
    if (!argv || !argv[1])
    {
        target = env_get(*env, "HOME");
        if (!target)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", 2);
            return (1);
        }
    }
    else
        target = argv[1];
    oldpwd = getcwd(NULL, 0); 
    if (chdir(target) != 0)
    {
        if (oldpwd)
            free(oldpwd);
        print_cd_error(target);
        return (1);
    }
    newpwd = getcwd(NULL, 0);
    if (oldpwd)
    {
        env_set(env, "OLDPWD", oldpwd);
        free(oldpwd);
    }
    if (newpwd)
    {
        env_set(env, "PWD", newpwd);
        free(newpwd);
    }
    else
    {
        ft_putstr_fd("minishell: cd: getcwd: ", 2);
        ft_putstr_fd((char *)strerror(errno), 2);
        ft_putstr_fd("\n", 2);
    }
    return (0);
}
