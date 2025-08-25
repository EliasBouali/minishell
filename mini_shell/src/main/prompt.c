#include "../include/minishell.h"

void prompt_loop(char **envp)
{
    char *line;
    t_command cmd;
    t_env *env;

    env = init_env(envp);
    if (!env)
    {
      ft_putstr_fd("minishell: failed to init environment\n", 2);
      return ;
    }
    setup_prompt_signals();
    while (1)
    {
        line = readline("minishell$ ");
        if (!line)
        {
            ft_putendl_fd("exit", 1);
            g_exit_code = 0;
            break;
        }
        if (is_blank_line(line))
        {
            free(line);
            continue;
        }
        add_history(line);
        cmd.args = ft_split(line, ' ');
        if (cmd.args && cmd.args[0] && cmd_is_builtin(cmd.args[0]))
          g_exit_code = exec_builtin(cmd.args, &env);
        else
          handle_command(&cmd, env);
        free_split(cmd.args);
        free(line);
    }
    free_env(&env);
}
