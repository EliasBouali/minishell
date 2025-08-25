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
#include <limits.h>



typedef struct s_command
{
  char **args;
}       t_command;

extern int g_exit_code;

typedef struct s_env
{
  char *value;
  char *name;
  struct s_env *next;
} t_env;

// fonction dans le dossier utils, fichier utils.c
int	ft_strncmp(const char *s1, const char *s2, size_t n);
size_t ft_strlen(const char *s);
void free_split(char **split);
char    *ft_strchr(const char *s, int c);
void    ft_putstr_fd(char *s, int fd);
char    *ft_strdup(const char *s);


// fonction dans le dossier utils, fichier second_utils.c:
void    ft_putendl_fd(char *s, int fd);
int is_blank_line(char *s);

// fonction dans le dossier utils, fichier ft_strjoin.c:
char	*ft_strjoin(char const *s1, char const *s2);

// fontion dans le dossier utils, fichier ft_split.c;
char	**ft_split(char const *s, char c);

// fonction dans le dossier executor fichier exec.c :
char *get_path_to_cmd(char *cmd, const char *path_var);
void handle_command(t_command *cmd, t_env *env);

// fonction dans ledossier signals fichier signals.c:
void prompt_signal_handler(int sign);
void setup_prompt_signals(void);

// fonction dans le dossier main fichier prompt.c:
void prompt_loop(char **env);

// fonction dans le dossier src sous dossier builtins :
int ft_echo(char **argv, t_env **env);
int ft_pwd(char **argv, t_env **env);
int ft_env(char **argv, t_env **env);
int ft_cd(char **argv, t_env **env);
int cmd_is_builtin(const char *cmd);
int exec_builtin(char **argv, t_env **env);
int ft_export(char **argv, t_env **env);
int ft_exit(char **argv, t_env **env);
int ft_unset(char **argv, t_env **env);

// fonction dans le dossier utils fichier three_utils.c :
int is_valid_name(const char *s);
t_env *find_by_name(t_env *env, const char *name);
void free_envp_array(char **arr);

// fonction dans le dossier env :
      //fichier env_init.c:
      t_env	*init_env(char **envp_src);
      // fichier env_node_new.c:
      t_env *env_node_new(const char *name, const char *value);
      // fichier env_set.c:
      int env_set(t_env **env, const char *name, const char *value);
      // fichier env_to_envp.c:
      char	**env_to_envp(t_env *env);
      // fichier env_unset.c:
      int env_unset(t_env **env, const char *name);
      // fichier free_env.c:
      void free_env(t_env **env);
      // get_env.c:
      const char *env_get(t_env *env, const char *name);





#endif
