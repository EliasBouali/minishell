#include "../../include/minishell.h"

int parse_is_valid(const char *s, long long *val_out)
{
  int i;
  int sign;
  long long res;
  int has_digit;
  int digit;

  if (!s || !val_out)
    return 0;
  i = 0;
  sign = 1;
  res = 0;
  has_digit = 0;
  while (s[i] == ' ' || s[i] == '\t')
    i++;
  if (s[i] == '+' || s[i] == '-')
  {
    if (s[i] == '-')
      sign = -1;
    i++;
  }
  if (!(s[i] >= '0' && s[i] <= '9'))
    return 0;
  while (s[i] >= '0' && s[i] <= '9')
  {
    digit = s[i] - '0';
    if (res < (LLONG_MIN + digit) / 10)
      return (0);
    res = res * 10 - digit;
    has_digit = 1;
    i++;
  }
  if (!has_digit)
    return (0);
  while (s[i] == ' ' || s[i] == '\t')
    i++;
  if (s[i] != '\0')
    return (0);
  if (sign == 1)
  {
    if (res == LLONG_MIN)
      return (0);
    *val_out = -res;
  }
  else
    *val_out = res;
  return (1);
}

int ft_exit(char **argv)
{
  long long value;

  if (!argv || !argv[1])
  {
    ft_putendl_fd("exit", 1);
    exit(g_exit_code);
  }
  if (!parse_is_valid(argv[1], &value))
  {
    ft_putendl_fd("exit", 1);
    ft_putstr_fd("minishell: exit: ", 2);
    ft_putstr_fd(argv[1], 2);
    ft_putendl_fd(": numeric argument required", 2);
    exit(255);
  }
  if (argv[2])
  {
    ft_putstr_fd("minishell: exit: too many arguments\n", 2);
    return (1);
  }
  ft_putendl_fd("exit", 1);
  exit((unsigned char)value);
  return (0);
}
