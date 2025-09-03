#include "../../include/minishell.h"

static int is_n_flag(const char *s)
{
  int i;

  if (!s || s[0] != '-' || s[1] != 'n')
    return (0);
  i = 2;
  while (s[i] == 'n')
    i++;
  return (s[i] == '\0');
}

static void print_args(char **argv, int i)
{
  while (argv[i])
  {
    ft_putstr_fd(argv[i], 1);
    if (argv[i + 1])
      ft_putstr_fd(" ", 1);
    i++;
  }
}

int	ft_echo(char **argv, t_env **env)
{
	int i;
	int new_line;

	(void)env;
	i = 1;
	new_line = 1;
	while (argv[i] && is_n_flag(argv[i]))
	{
		new_line = 0;
		i++;
	}
	print_args(argv, i);
	if (new_line)
		ft_putstr_fd("\n", 1);
	return (0);
}
