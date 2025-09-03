#include "../include/minishell.h"

typedef struct s_pipeline_ctx
{
	int		**pipes;
	int		n;
	t_env	*env;
	t_command *head;
}	t_pipeline_ctx;


static int	setup_child_io(int idx, t_command *cmd, t_pipeline_ctx *cx)
{
	restore_signals_for_child();
	connect_pipes_for_child(idx, cx->n, cx->pipes);
	close_all_pipes(cx->pipes, cx->n);
	free_pipes(cx->pipes, cx->n);
	if (apply_redirections(cmd, &cx->env) == -1)
	{
		if (g_exit_code == 0)
			g_exit_code = 1;
		return (-1);
	}
	return (0);
}


static void	child_exec(int idx, t_command *cmd, t_pipeline_ctx *cx)
{
	char	*name;
	char	*path;
	int		own;

	if (setup_child_io(idx, cmd, cx) == -1)
		exit(g_exit_code);
	name = NULL;
	path = NULL;
	own = 0;
	if (cmd && cmd->args)
		name = cmd->args[0];
	if (!name || !*name)
		exit(0);
	if (cmd_is_builtin(name))
		exit(exec_builtin(cmd->args, &cx->env));
	if (resolve_command(name, cx->env, &path, &own) != 0)
		exit(g_exit_code);
	exec_external_child(path, cmd->args, cx->env, own);
}

static int	launch_children(t_pipeline_ctx *cx, pid_t *pids)
{
	int			i;
	t_command	*cmd;

	i = 0;
	while (i < cx->n)
	{
		cmd = get_cmd_position(cx->head, i);
		pids[i] = fork();
		if (pids[i] < 0)
			return (handle_spawn_error(i, cx->n, pids, cx->pipes));
		if (pids[i] == 0)
			child_exec(i, cmd, cx);
		i++;
	}
	return (0);
}


int	execute_pipeline(t_command *head, t_env *env)
{
	int				n;
	int				**pipes;
	pid_t			*pids;
	t_pipeline_ctx	cx;
	int				rc;

	n = count_cmd(head);
	if (n < 2)
    return (handle_command(head, env),g_exit_code);
	if (setup_pipes_and_pids(n, &pipes, &pids) < 0)
		return (g_exit_code = 1);
	cx.pipes = pipes;
	cx.n = n;
	cx.env = env;
	cx.head = head;
	rc = launch_children(&cx, pids);
	close_all_pipes(pipes, n);
	free_pipes(pipes, n);
	if (rc < 0)
    return (g_exit_code);
	rc = wait_pipeline(pids, n);
	free(pids);
	return (rc);
}
