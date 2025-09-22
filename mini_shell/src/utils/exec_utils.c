/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:46:41 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:46:42 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_error(const char *name, const char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)name, 2);
	ft_putendl_fd((char *)msg, 2);
}

static char	*join_dir_cmd(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

char	*get_valid_exec_path(char **path_split, char *cmd)
{
	int		i;
	char	*full;

	i = 0;
	if (!path_split)
		return (NULL);
	while (path_split[i])
	{
		full = join_dir_cmd(path_split[i], cmd);
		if (!full)
		{
			free_split(path_split);
			return (NULL);
		}
		if (access(full, X_OK) == 0)
		{
			free_split(path_split);
			return (full);
		}
		free(full);
		i++;
	}
	free_split(path_split);
	return (NULL);
}

static char	*ret_cand_free_dirs(char *cand, char **dirs)
{
	free_split(dirs);
	return (cand);
}

char	*get_path_to_cmd(char *cmd, const char *path_var)
{
	char	**dirs;
	char	*cand;
	int		i;

	if (!cmd || !path_var || !*path_var)
		return (NULL);
	dirs = ft_split(path_var, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		cand = join_dir_cmd(dirs[i], cmd);
		if (!cand)
			break ;
		if (access(cand, X_OK) == 0)
			return (ret_cand_free_dirs(cand, dirs));
		free(cand);
		i++;
	}
	free_split(dirs);
	return (NULL);
}
