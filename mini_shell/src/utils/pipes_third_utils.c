/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_third_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:48:01 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:48:03 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	wait_pipeline(pid_t *pids, int n)
{
	int	i;
	int	status;
	int	last;

	i = 0;
	last = 0;
	while (i < n)
	{
		if (waitpid(pids[i], &status, 0) == pids[n - 1])
			last = status;
		i++;
	}
	if (WIFEXITED(last))
		g_exit_code = WEXITSTATUS(last);
	else if (WIFSIGNALED(last))
		g_exit_code = 128 + WTERMSIG(last);
	else
		g_exit_code = 1;
	return (g_exit_code);
}

int	resolve_slash_command(const char *name, char **out_path, int *path_owned)
{
	int	rc;

	*out_path = NULL;
	*path_owned = 0;
	rc = path_checks_for_slash_cmd(name);
	if (rc != 0)
	{
		g_exit_code = rc;
		return (-1);
	}
	*out_path = (char *)name;
	return (0);
}

int	resolve_in_path(const char *name, t_env *env, char **out_path,
		int *path_owned)
{
	const char	*path_var;
	char		*path;

	*out_path = NULL;
	*path_owned = 0;
	path_var = env_get(env, "PATH");
	path = get_path_to_cmd((char *)name, path_var);
	if (!path)
	{
		print_cmd_not_found(name);
		g_exit_code = 127;
		return (-1);
	}
	*out_path = path;
	*path_owned = 1;
	return (0);
}

int	resolve_command(const char *name, t_env *env, char **out_path,
		int *path_owned)
{
	if (ft_strchr(name, '/'))
		return (resolve_slash_command(name, out_path, path_owned));
	return (resolve_in_path(name, env, out_path, path_owned));
}

void	exec_external_child(char *path, char **argv, t_env *env, int own)
{
	char	**envp;

	envp = env_to_envp(env);
	if (!envp)
		_exit(1);
	execve(path, argv, envp);
	if (own)
		free(path);
	free_envp_array(envp);
	if (errno == EACCES)
		_exit(126);
	_exit(127);
}
