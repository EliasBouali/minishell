/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_third_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 15:13:26 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/22 15:13:28 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	save_fds(int *in, int *out)
{
	*in = dup(STDIN_FILENO);
	*out = dup(STDOUT_FILENO);
	if (*in == -1 || *out == -1)
	{
		perror("minishell: dup");
		if (*in != -1)
			close(*in);
		if (*out != -1)
			close(*out);
		return (-1);
	}
	return (0);
}
