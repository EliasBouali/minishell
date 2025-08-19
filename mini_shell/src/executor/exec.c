#include "../include/minishell.h"
int g_exit_code = 0;

char *get_env(char *nam, char **env)
{
	int i = 0;
	size_t len = ft_strlen(nam);

	while (env && env[i])
	{
		if (ft_strncmp(env[i], nam, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}


char *get_valid_exec_path(char **path_split, char *cmd)
{
  int i;
  char *tmp;
  char *full_path;

  i = 0;
  while(path_split[i])
  {
    tmp = ft_strjoin(path_split[i], "/");
    if (!tmp)
    {
      free_split(path_split);
      return (NULL);
    }
    full_path = ft_strjoin(tmp, cmd);
    free(tmp);
    if (!full_path)
    {
      free_split(path_split);
      return (NULL);
    }
    if (access(full_path, X_OK) == 0)
    {
      free_split(path_split);
      return (full_path);
    }
    free(full_path);
    i++;
  }
  free_split(path_split);
  return (NULL);
}

char *get_path_to_cmd(char *cmd, char **env)
{
  char *env_path;
  char **path_split;

  env_path = get_env("PATH", env);
  if (!env_path)
    return (NULL);
  path_split = ft_split(env_path, ':');
    if (!path_split)
      return (NULL);
  return (get_valid_exec_path(path_split, cmd));
}

void handle_command(t_command *cmd, char **env)
{
  char *path;
  pid_t pid;
  char *cmd_name ;

  if (!cmd || !cmd->args || !cmd->args[0])
    return ;
  cmd_name = cmd->args[0];
  if (ft_strchr(cmd_name, '/'))
  {
    struct  stat st;
    if (access(cmd_name, F_OK) != 0)
    {
      ft_putstr_fd("minishell: ", 2);
      ft_putstr_fd(cmd_name, 2);
      ft_putendl_fd(": No such file or directory", 2);
      g_exit_code = 127;
      return ;
    }
    if (stat(cmd_name, &st) == 0 && S_ISDIR(st.st_mode))
      {
			  ft_putstr_fd("minishell: ", 2);
			  ft_putstr_fd(cmd_name, 2);
			  ft_putendl_fd(": Is a directory", 2);
			  g_exit_code = 126;
			  return;
		  }
    if (access(cmd_name, X_OK) != 0)
    {
      ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_name, 2);
			ft_putendl_fd(": Permission denied", 2);
      g_exit_code = 126;
      return ;
    }
    path = cmd_name;
  }
  else
  {
    path = get_path_to_cmd(cmd->args[0], env);
    if (!path)
    {
      ft_putstr_fd("minishell: ", 2);
      ft_putstr_fd(cmd_name, 2);
      ft_putendl_fd(": command not found", 2);
      g_exit_code = 127;
      return ;
    }
  }

  pid = fork();
  if (pid == -1)
  {
    perror("minishell");
    if (path != cmd_name)
      free(path);
    return;
}
if (pid == 0)
{
  signal(SIGINT, SIG_DFL);
  signal(SIGQUIT, SIG_DFL);
  execve(path, cmd->args, env);
  perror("minishell");
  if (path != cmd_name)
    free(path);
  exit(126);
}
int status = 0;
waitpid(pid, &status, 0);

if (WIFEXITED(status))
    g_exit_code = WEXITSTATUS(status);
else if (WIFSIGNALED(status))
    g_exit_code = 128 + WTERMSIG(status);

if (path != cmd_name)
    free(path);
return;

}
