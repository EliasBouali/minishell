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

static int	check_slash_cmd_fs(const char *name)
{
	struct stat	st;

	if (access(name, F_OK) != 0)
	{
		print_error(name, ": No such file or directory");
		g_exit_code = 127;
		return (-1);
	}
	if (stat(name, &st) == 0 && S_ISDIR(st.st_mode))
	{
		print_error(name, ": Is a directory");
		g_exit_code = 126;
		return (-1);
	}
	if (access(name, X_OK) != 0)
	{
		print_error(name, ": Permission denied");
		g_exit_code = 126;
		return (-1);
	}
	return (0);
}

static char	*resolve_cmd_path(char *name, t_env *env, int *path_owned)
{
	const char	*path_var;
	char		*path;

	*path_owned = 0;
	if (ft_strchr(name, '/'))
	{
		if (check_slash_cmd_fs(name) < 0)
			return (NULL);
		return (name);
	}
	path_var = env_get(env, "PATH");
	path = get_path_to_cmd(name, path_var);
	if (!path)
	{
		print_error(name, ": command not found");
		g_exit_code = 127;
		return (NULL);
	}
	*path_owned = 1;
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

	if (!build_child_envp(env))
	{
		if (path_owned)
			free(path);
		g_exit_code = 1;
		return ;
	}
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

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	name = cmd->args[0];
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
