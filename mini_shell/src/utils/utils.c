#include "../include/minishell.h"


int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (!s1 || !s2)
		return (0);
	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] && s2[i] && i < (n - 1) && s1[i] == s2[i])
	{
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void free_split(char **split)
{
  int i;

  if (!split)
    return ;
  i = 0;
  while (split[i])
  {
    free(split[i]);
    i++;
  }
  free(split);
}

char    *ft_strchr(const char *s, int c)
{
    int i;

    if (!s)
        return (0);
    i = 0;
    while (s[i])
    {
        if (s[i] == (char)c)
            return ((char *)&s[i]);
        i++;
    }
    if (c == '\0')
        return ((char *)&s[i]);
    return (NULL);
}

void    ft_putstr_fd(char *s, int fd)
{
    int i;

    if (!s)
        return ;
    i = 0;
    while (s[i])
    {
        write(fd, &s[i], 1);
        i++;
    }
}
