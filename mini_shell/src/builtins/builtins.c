#include "../../include/minishell.h"

typedef int (*t_builtin_fn) (char **argv);

typedef struct s_builtin {
    const char *name;
    t_builtin_fn function;
} t_builtin;

static const t_builtin make_builtin[] = {
  {"echo", ft_echo},
  {"pwd", ft_pwd},
  {"env", ft_env},
  {NULL, NULL}
};

int cmd_is_builtin(const char *cmd)
{
  int i;

  i = 0;
  if (!cmd)
    return (0);
    while (make_builtin[i].name)
    {
      if (ft_strncmp(cmd, make_builtin[i].name,ft_strlen(make_builtin[i].name) + 1) == 0)
        return (1);
      i++;
    }
    return (0);
}

int exec_builtin(char **argv)
{
  int i;

  i = 0;
  if (!argv || !argv[0])
    return (0);
  while (make_builtin[i].name)
  {
    if (ft_strncmp(argv[0], make_builtin[i].name, ft_strlen(make_builtin[i].name) + 1) == 0)
    {
      return (make_builtin[i].function(argv));
    }
    i++;
  }
  return (0);
}
