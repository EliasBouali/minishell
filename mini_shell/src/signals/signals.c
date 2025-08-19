#include "../include/minishell.h"

void prompt_signal_handler(int sign)
{
    if (sign == SIGINT)
    {
        write(1, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
        g_exit_code = 130;
    }
}

void setup_prompt_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    // SIGINT
    sa_int.sa_handler = prompt_signal_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);

    // SIGQUIT
    sa_quit.sa_handler = SIG_IGN;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;
    sigaction(SIGQUIT, &sa_quit, NULL);
}
