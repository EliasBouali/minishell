#include "../../include/minishell.h"

int	ft_pwd(char **argv, t_env **env)
{
	char	*cwd;

	(void)argv;
  (void)env;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: pwd");
		return (1);
	}
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
	free(cwd);
	return (0);
}
