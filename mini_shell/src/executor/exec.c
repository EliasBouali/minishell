/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:42:29 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:42:30 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int			g_exit_code = 0;

static char	*resolve_cmd_path(char *name, t_env *env, int *path_owned)
{
	const char	*path_var;
	char		**dirs;
	char		*path;

	*path_owned = 0;
	if (ft_strchr(name, '/'))
	{
		if (check_slash_cmd_fs(name) < 0)
			return (NULL);
		return (name);
	}
	path_var = env_get(env, "PATH");
	if (!path_var || !*path_var)
		return (cmd_not_found(name));
	dirs = ft_split(path_var, ':');
	if (!dirs)
		return (alloc_fail());
	path = path_from_dirs(name, dirs, path_owned);
	if (!path)
		return (cmd_not_found(name));
	return (path);
}

static void	parent_after_fork(pid_t pid, char *path, int path_owned)
{
	int	status;

	waitpid(pid, &status, 0);
	if (path_owned)
		free(path);
	if (WIFEXITED(status))
		g_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_code = 128 + WTERMSIG(status);
	else
		g_exit_code = 1;
}

static void	spawn_and_exec(char *path, int path_owned, t_command *cmd,
		t_env *env)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell");
		if (path_owned)
			free(path);
		g_exit_code = 1;
		return ;
	}
	if (pid == 0)
		child_exec_sequence(cmd, env, path, path_owned);
	parent_after_fork(pid, path, path_owned);
}

void	handle_command(t_command *cmd, t_env *env)
{
	char	*name;
	char	*path;
	int		path_owned;

	if (!cmd || !cmd->ARGS || !cmd->ARGS[0])
		return ;
	name = cmd->ARGS[0];
	if (cmd_is_builtin(name))
	{
		g_exit_code = run_builtin_in_parent(cmd, &env);
		return ;
	}
	path = resolve_cmd_path(name, env, &path_owned);
	if (!path)
		return ;
	spawn_and_exec(path, path_owned, cmd, env);
}
