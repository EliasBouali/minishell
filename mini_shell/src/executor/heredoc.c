#include "../include/minishell.h"


int open_heredoc(const char *delimiteur, int *out_fd, t_env **env)
{
    int   pfd[2];
    pid_t pid;

    (void)env;
    if (pipe(pfd) == -1)
    {
        perror("minishell: pipe");
        return (-1);
    }
    
    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork");
        close(pfd[0]);
        close(pfd[1]);
        return (-1);
    }
    if (pid == 0)
    {
        char *line;
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_IGN);
        close(pfd[0]);
        while (1)
        {
            line = readline("> ");
            if (!line
                || ft_strncmp(line, delimiteur, ft_strlen(delimiteur) + 1) == 0)
            {
                free(line);
                break;
            }
            write(pfd[1], line, ft_strlen(line));
            write(pfd[1], "\n", 1);
            free(line);
        }
        close(pfd[1]);
        exit(0);
    }
    else
    {
        int status;
        close(pfd[1]);
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("minishell: waitpid");
            close(pfd[0]);
            return (-1);
        }
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
            g_exit_code = 130;
            close(pfd[0]);
            return (-1);
        }
        *out_fd = pfd[0];
        return (0);
    }
}
