/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_envp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:40:02 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:40:03 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static size_t  env_count_with_value(t_env *env)
{
    size_t n = 0;
    while (env)
    {
        if (env->value != NULL)
            n++;
        env = env->next;
    }
    return n;
}

/* Join name + '=' + value safely into a newly malloc'ed C-string */
static char    *join_kv(const char *name, const char *value)
{
    size_t ln, lv;
    char   *s;

    if (!name || !value)
        return NULL;
    ln = ft_strlen(name);
    lv = ft_strlen(value);
    s = (char *)malloc(ln + 1 /* '=' */ + lv + 1 /* '\0' */);
    if (!s)
        return NULL;
    ft_memcpy(s, name, ln);
    s[ln] = '=';
    ft_memcpy(s + ln + 1, value, lv);
    s[ln + 1 + lv] = '\0';
    return s;
}
/*
static void    free_envp_array(char **arr)
{
    size_t i = 0;

    if (!arr)
        return;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}
	*/
char    **env_to_envp(t_env *env)
{
    size_t  n = env_count_with_value(env), i = 0;
    char  **arr = (char **)malloc(sizeof(char *) * (n + 1));

    if (!arr)
        return (ft_putstr_fd("minishell: env alloc error\n", 2), NULL);
    while (env)
    {
        if (env->value)
        {
            arr[i] = join_kv(env->name, env->value);
            if (!arr[i])
            {
                while (i) free(arr[--i]);
                free(arr);
                ft_putstr_fd("minishell: env alloc error\n", 2);
                return (NULL);
            }
            i++;
        }
        env = env->next;
    }
    arr[i] = NULL;
    return (arr);
}

