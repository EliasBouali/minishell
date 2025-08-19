#include "../include/minishell.h"

int ft_echo(char **argv)
{
  int i;
  int new_line;
  int j;

  i = 1;
  new_line = 1;
  while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
  {
    j = 2;
    while(argv[i][j] == 'n')
      j++;
    if (argv[i][j] != '\0')
      break;
    new_line = 0;
    i++;
  }
  while (argv[i])
  {
    ft_putstr_fd(argv[i], 1);
    if (argv[i + 1])
      ft_putstr_fd(" ", 1);
    i++;
  }
  if (new_line)
    ft_putstr_fd("\n", 1);
    return 0;
}
