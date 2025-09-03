#include "../include/minishell.h"

int create_pipes(int **pipes, int number_of_cmd)
{
  int i;

  if (!pipes)
    return (-1);
  if (number_of_cmd < 2)
    return (0);
  i = 0;
  while (i < number_of_cmd - 1)
  {
    if (pipe(pipes[i]) < 0)
      return (-1);
    i++;
  }
  return (0);
}

void connect_pipes_for_child(int cmd_index, int number_of_cmd, int **pipes)
{
  if (cmd_index > 0)
  {
    if (dup2(pipes[cmd_index - 1][0], STDIN_FILENO) == -1)
      exit(1);
  }
  if (cmd_index < number_of_cmd - 1)
  {
    if (dup2(pipes[cmd_index][1], STDOUT_FILENO) == -1)
      exit(1);
  }
}

t_command *get_cmd_position(t_command *head, int idx_cmd_pipeline)
{
  while (head && idx_cmd_pipeline > 0)
  {
    head = head->next;
    idx_cmd_pipeline--;
  }
  return (head);
}

void print_cmd_not_found(const char *name)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd((char *)name, 2);
    ft_putendl_fd(": command not found", 2);
}

int path_checks_for_slash_cmd(const char *cmd_name)
{
    struct stat st;

    if (access(cmd_name, F_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd((char *)cmd_name, 2);
        ft_putendl_fd(": No such file or directory", 2);
        return 127;
    }
    if (stat(cmd_name, &st) == 0 && S_ISDIR(st.st_mode))
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd((char *)cmd_name, 2);
        ft_putendl_fd(": Is a directory", 2);
        return 126;
    }
    if (access(cmd_name, X_OK) != 0)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd((char *)cmd_name, 2);
        ft_putendl_fd(": Permission denied", 2);
        return 126;
    }
    return 0;
}
