#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>



typedef struct s_command
{
  char **args;
}       t_command;

extern int g_exit_code;

// fonction dans le dossier utils, fichier utils.c
int	ft_strncmp(const char *s1, const char *s2, size_t n);
size_t ft_strlen(const char *s);
void free_split(char **split);
char    *ft_strchr(const char *s, int c);
void    ft_putstr_fd(char *s, int fd);

// fonction dans le dossier utils, fichier second_utils.c:
void    ft_putendl_fd(char *s, int fd);
int is_blank_line(char *s);

// fonction dans le dossier utils, fichier ft_strjoin.c:
char	*ft_strjoin(char const *s1, char const *s2);

// fontion dans le dossier utils, fichier ft_split.c;
char	**ft_split(char const *s, char c);

// fonction dans le dossier executor fichier exec.c :
void handle_command(t_command *cmd, char **env);

// fonction dans ledossier signals fichier signals.c:
void prompt_signal_handler(int sign);
void setup_prompt_signals(void);

// fonction dans le dossier main fichier prompt.c:
void prompt_loop(char **env);

// fonction dans le dossier src sous dossier builtins :
int ft_echo(char **argv);
int ft_pwd(char **argv);
int ft_env(char **argv);
int cmd_is_builtin(const char *cmd);
int exec_builtin(char **argv);






#endif
