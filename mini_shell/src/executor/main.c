#include "../include/minishell.h"

extern int rl_catch_signals;

int main(int argc, char **argv, char **env)
{
    (void)argc;
    (void)argv;

    // Empêcher readline de gérer elle-même les signaux
    rl_catch_signals = 0;

    // Lancer la boucle du shell
    prompt_loop(env);

    return (g_exit_code);
}
