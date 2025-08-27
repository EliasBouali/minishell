#include "../include/minishell.h"

// parsing fait par chat le tant que Stephane finisse son parsing

static char *ft_itoa(int n)
{
    char buf[12];
    int len = 0;
    int tmp = n;
    char *res;

    if (n == 0)
        return ft_strdup("0");

    while (tmp > 0) {
        buf[len++] = '0' + (tmp % 10);
        tmp /= 10;
    }
    res = (char *)malloc(len + 1);
    if (!res)
        return NULL;
    for (int i = 0; i < len; i++)
        res[i] = buf[len - 1 - i];
    res[len] = '\0';
    return res;
}
static void parse_line_naive(const char *line, t_command *cmd)
{
    char    **tokens = NULL;
    t_redir *r_head = NULL, *r_tail = NULL;
    char    **argv_tmp = NULL;
    int     argc_tmp = 0;
    int     i, j;

    cmd->args = NULL;
    cmd->redir = NULL;

    tokens = ft_split(line, ' ');
    if (!tokens)
        return;

    /* 1) Compter les tokens non-redirections */
    for (i = 0; tokens[i]; )
    {
        if (!ft_strncmp(tokens[i], ">>", 3) || !ft_strncmp(tokens[i], "<<", 3) ||
            !ft_strncmp(tokens[i], ">", 2)  || !ft_strncmp(tokens[i], "<", 2))
        {
            if (!tokens[i + 1]) {
                ft_putendl_fd("minishell: syntax error near unexpected token", 2);
                break;
            }
            i += 2; /* sauter opérateur + cible */
            continue;
        }
        argc_tmp++;
        i++;
    }

    if (argc_tmp > 0) {
        argv_tmp = (char **)malloc(sizeof(char*) * (argc_tmp + 1));
        if (!argv_tmp) {
            ft_putendl_fd("minishell: alloc error", 2);
            free_split(tokens);
            return;
        }
    }

    /* 2) Construire argv_tmp et la liste des redirections */
    j = 0;
    for (i = 0; tokens[i]; )
    {
        t_redir *r = NULL;

        if (!ft_strncmp(tokens[i], ">>", 3) && tokens[i + 1]) {
            r = (t_redir*)malloc(sizeof(*r));
            if (!r) { ft_putendl_fd("minishell: alloc error", 2); goto fail; }
            r->type = REDIR_APPEND;
            r->target = ft_strdup(tokens[i + 1]);
            r->fd = -1; r->next = NULL;
        }
        else if (!ft_strncmp(tokens[i], "<<", 3) && tokens[i + 1]) {
            r = (t_redir*)malloc(sizeof(*r));
            if (!r) { ft_putendl_fd("minishell: alloc error", 2); goto fail; }
            r->type = REDIR_HEREDOC;
            r->target = ft_strdup(tokens[i + 1]);
            r->fd = -1; r->next = NULL;
        }
        else if (!ft_strncmp(tokens[i], ">", 2) && tokens[i + 1]) {
            r = (t_redir*)malloc(sizeof(*r));
            if (!r) { ft_putendl_fd("minishell: alloc error", 2); goto fail; }
            r->type = REDIR_OUTPUT;
            r->target = ft_strdup(tokens[i + 1]);
            r->fd = -1; r->next = NULL;
        }
        else if (!ft_strncmp(tokens[i], "<", 2) && tokens[i + 1]) {
            r = (t_redir*)malloc(sizeof(*r));
            if (!r) { ft_putendl_fd("minishell: alloc error", 2); goto fail; }
            r->type = REDIR_INPUT;
            r->target = ft_strdup(tokens[i + 1]);
            r->fd = -1; r->next = NULL;
        }
        else {
            /* token normal → copie; expansion minimale de "$?" */
            if (argv_tmp) {
                if (!ft_strncmp(tokens[i], "$?", 3)) {
                    char *num = ft_itoa(g_exit_code);
                    argv_tmp[j++] = num ? num : ft_strdup("$?");
                } else {
                    argv_tmp[j++] = ft_strdup(tokens[i]);
                }
            }
            i++;
            continue;
        }

        if (!r || !r->target) { ft_putendl_fd("minishell: alloc error", 2); if (r) free(r); goto fail; }
        if (!r_head) r_head = r; else r_tail->next = r; r_tail = r;
        i += 2; /* saute opérateur + cible */
    }

    if (argv_tmp)
        argv_tmp[j] = NULL;

    free_split(tokens);
    cmd->args = argv_tmp;
    cmd->redir = r_head;
    return;

fail:
    /* Nettoyage en cas d'erreur d'alloc pendant la 2e passe */
    if (argv_tmp) {
        while (j-- > 0) free(argv_tmp[j]);
        free(argv_tmp);
    }
    {
        t_redir *x = r_head, *tmp;
        while (x) { tmp = x->next; if (x->target) free(x->target); free(x); x = tmp; }
    }
    free_split(tokens);
    cmd->args = NULL;
    cmd->redir = NULL;
}


static void free_redir(t_redir **head)
{
  t_redir *redirection;
  t_redir *tmp;

  if (!head)
    return ;
  redirection = *head;
  while (redirection)
  {
    tmp = redirection->next;
    if (redirection->target)
      free(redirection->target);
    free(redirection);
    redirection = tmp;
  }
  *head = NULL;
}
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
        parse_line_naive(line, &cmd);
        handle_command(&cmd, env);
        free_split(cmd.args);
        free_redir(&cmd.redir);
        free(line);
    }
    free_env(&env);
}
