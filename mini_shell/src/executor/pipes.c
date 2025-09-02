#include "../include/minishell.h"

static int wait_pipeline(pid_t *pids, int number_of_cmd)
{
  int i;
  int status;
  int last;

  i = 0;
  status = 0;
  last = 0;
  while (i < number_of_cmd)
  {
    if (waitpid(pids[i], &status, 0) == pids[number_of_cmd - 1])
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

typedef struct  s_pipeline_ctx
{
  int **pipes;
  int i;
  t_env *env;
} t_pipeline_ctx;

static void child_exec(int i, t_command *cmd, t_pipeline_ctx *cx)
{
    char       *name;
    const char *path_var;
    char       *path;
    char      **envp_child;
    int         rc;

    restore_signals_for_child();
    connect_pipes_for_child(i, cx->i, cx->pipes);
    close_all_pipes(cx->pipes, cx->i);
    free_pipes(cx->pipes, cx->i);
    if (apply_redirections(cmd, &cx->env) == -1)
    {
        if (g_exit_code == 0)
            g_exit_code = 1;
        exit(g_exit_code);
    }
    if (cmd && cmd->args)
        name = cmd->args[0];
    else
        name = NULL;
    if (name && cmd_is_builtin(name))
    { rc = exec_builtin(cmd->args, &cx->env); exit(rc); }
    if (!name || !*name) exit(0);
    if (ft_strchr(name, '/'))
    { rc = path_checks_for_slash_cmd(name); if (rc != 0) exit(rc); path = (char *)name; }
    else
    {
        path_var = env_get(cx->env, "PATH");
        path = get_path_to_cmd(name, path_var);
        if (!path) { print_cmd_not_found(name); exit(127); }
    }
    envp_child = env_to_envp(cx->env);
    if (!envp_child) exit(1);
    execve(path, cmd->args, envp_child);
    if (errno == EACCES) exit(126);
    exit(127);
}

int execute_pipeline(t_command *head, t_env *env)
{
    int       pipelines_number;
    int     **pipes;
    pid_t    *pids;
    int       i;
    t_pipeline_ctx cx;

    pipelines_number = count_cmd(head);
    if (pipelines_number < 2) { handle_command(head, env); return g_exit_code; }
    pipes = alloc_pipes(pipelines_number);
    if ((pipelines_number > 1 && !pipes) || (pipelines_number > 1 && create_pipes(pipes, pipelines_number) < 0))
        return (g_exit_code = 1);
    pids = (pid_t *)malloc(sizeof(pid_t) * pipelines_number);
    if (!pids) { close_all_pipes(pipes, pipelines_number); free_pipes(pipes, pipelines_number); return (g_exit_code = 1); }
    cx.pipes = pipes; cx.i = pipelines_number; cx.env = env;
    i = 0;
    while (i < pipelines_number)
    {
        t_command *cmd = get_cmd_position(head, i);
        pids[i] = fork();
        if (pids[i] < 0)
        { close_all_pipes(pipes, pipelines_number); while (--i >= 0) waitpid(pids[i], NULL, 0);
          free(pids); free_pipes(pipes, pipelines_number); return (g_exit_code = 1); }
        if (pids[i] == 0)
            child_exec(i, cmd, &cx);
        i++;
    }
    close_all_pipes(pipes, pipelines_number);
    free_pipes(pipes, pipelines_number);
    i = wait_pipeline(pids, pipelines_number);
    free(pids);
    return i;
}
