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

char	*join_dir_cmd(char *dir, char *cmd)
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
	while (path_split && path_split[i])
	{
		full = join_dir_cmd(path_split[i], cmd);
		if (!full)
			return (free_split(path_split), NULL);
		if (access(full, X_OK) == 0)
			return (free_split(path_split), full);
		free(full);
		i++;
	}
	free_split(path_split);
	return (NULL);
}

char	*get_path_to_cmd(char *cmd, const char *path_var)
{
	char	**split;

	if (!path_var || !*path_var)
		return (NULL);
	split = ft_split(path_var, ':');
	if (!split)
		return (NULL);
	return (get_valid_exec_path(split, cmd));
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
