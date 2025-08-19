#include "../include/minishell.h"

void    ft_putendl_fd(char *s, int fd)
{
    int i;

    i = 0;
    if (!s)
        return ;
    while (s[i])
    {
        write(fd, &s[i], 1);
        i++;
    }
    write(fd, "\n", 1);
}

int is_blank_line(char *s)
{
    int i = 0;

    if (!s)
        return (1);

    while (s[i])
    {
        if (s[i] != ' ' && s[i] != '\t')
            return (0);
        i++;
    }
    return (1);
}
