#include "../include/minishell.h"


char	*cmd_not_found(char *name)
{
	print_error(name, ": command not found");
	g_exit_code = 127;
	return (NULL);
}

char	*alloc_fail(void)
{
	g_exit_code = 1;
	return (NULL);
}

char	*path_from_dirs(const char *name, char **dirs, int *owned)
{
	int		i;
	char	*tmp;
	char	*cand;

	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		if (!tmp)
			break ;
		cand = ft_strjoin(tmp, name);
		free(tmp);
		if (!cand)
			break ;
		if (access(cand, X_OK) == 0)
		{
			free_split(dirs);
			*owned = 1;
			return (cand);
		}
		free(cand);
		i++;
	}
	free_split(dirs);
	return (NULL);
}