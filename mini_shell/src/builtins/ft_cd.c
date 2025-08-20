#include "../../include/minishell.h"

int ft_cd(char **argv)
{
  if (!argv[1])
    return (0);
  if (chdir(argv[1]) != 0)
  {
    ft_putstr_fd("minishell: cd:", 2);
    ft_putstr_fd(argv[1], 2);
    ft_putstr_fd(": ", 2);
    ft_putstr_fd(strerror(errno),2);
    ft_putstr_fd("\n", 2);
    return (1);
  }
  return (0);
}


