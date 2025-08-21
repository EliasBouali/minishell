#include "../../include/minishell.h"

int parse_is_valid(char *s, int val_out)
{
  int i;
  int sign;
  long long val;
  int digit;

  if (!s)
    return 0;
  i = 0;
  sign = 1;
  val = 0;
  digit = 0;
  while (s[i] == ' ' || s[i] == '\t')
    i++;
  if (s[i] == '+' || s[i] == '-')
  {
    if (s[i] == '-')
      sign = -1;
    i++;
  }
  if (s[i] >= '0' && s[i] <= '9')
    return 0;
    while (s[i] >= '0' && s[i] <= '9')
    {
      digit = s[i] - '0';
          if (sign == 1)
    {
      if (val > (LLONG_MAX - digit) / 10)
        return 0;
    }
    else
    {
      if (val > (-(LLONG_MIN) - digit) / 10)
        return 0;
    }
    val = val * 10 + digit;
      i++;
    }
}
