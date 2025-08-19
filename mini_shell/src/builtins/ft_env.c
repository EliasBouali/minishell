#include "../../include/minishell.h"

extern char **environ;

int	ft_env(char **argv)
{
	int	i;

	(void)argv;
	if (!environ)
		return (0);
	i = 0;
	while (environ[i])
	{
		if (ft_strchr(environ[i], '='))
		{
			ft_putstr_fd(environ[i], 1);
			ft_putstr_fd("\n", 1);
		}
		i++;
	}
	return (0);
}
