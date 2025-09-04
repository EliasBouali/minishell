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
#include <string.h>
#include <fcntl.h>


typedef struct s_env
{
  char *value;
  char *name;
  struct s_env *next;
} t_env;

typedef enum e_redir_type
{
  REDIR_INPUT,             // <
  REDIR_OUTPUT,           // >
  REDIR_APPEND,          // >>
  REDIR_HEREDOC         // <<
} t_redir_type;

typedef struct s_redir
{
  t_redir_type type;
  char *target;
  int fd;
  struct s_redir *next;
}   t_redir;

typedef struct s_command
{
  char **args;
  t_redir *redir;
  struct s_command *next;
}       t_command;

typedef struct s_linebuf
{
	char	*buf;
	size_t	len;
	size_t	cap;
}	t_linebuf;


#define HD_INIT_CAP 64;

extern int g_exit_code;


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
void handle_command(t_command *cmd, t_env *env);
// fonction dans le dossier executor fichier haredoc.c :
int open_heredoc(const char *delimiteur, int *out_fd, t_env **env);
// fonction dans le dossier executor fichier redirection.c:
int apply_redirections(t_command *cmd, t_env **env);

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
void    *ft_memcpy(void *dst, const void *src, size_t n);
long long	ft_atoll(const char *str);

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

// fonction dans le dossier utils sous_dossier pipes_utils.c:
void restore_signals_for_child(void);
int count_cmd(t_command *head);
int **alloc_pipes(int number_of_cmd);
void free_pipes(int **pipes, int number_of_cmd);
void close_all_pipes(int **pipes, int number_of_cmd);

// fonction dans le dossier utils fichier  pipes_seconde_utils.c:
int create_pipes(int **pipes, int number_of_cmd);
void connect_pipes_for_child(int cmd_index, int number_of_cmd, int **pipes);
t_command *get_cmd_position(t_command *head, int idx_cmd_pipeline);
void print_cmd_not_found(const char *name);
int path_checks_for_slash_cmd(const char *cmd_name);

// fonction dans le dossier utils fichier pipes_third_utils.c:
int	wait_pipeline(pid_t *pids, int n);
int	resolve_slash_command(const char *name, char **out_path, int *path_owned);
int	resolve_in_path(const char *name, t_env *env, char **out_path, int *path_owned);
int	resolve_command(const char *name, t_env *env, char **out_path, int *path_owned);
void	exec_external_child(char *path, char **argv, t_env *env, int own);

// fonction dans le dossier utils fichier pipes_four_utils.c:
int	setup_pipes_and_pids(int n, int ***pipes_out, pid_t **pids_out);
int	handle_spawn_error(int i, int n, pid_t *pids, int **pipes);

// fonction dans le dossier utils fichier exec_utils.c:
void	print_error(const char *name, const char *msg);
char	*join_dir_cmd(char *dir, char *cmd);
char	*get_valid_exec_path(char **path_split, char *cmd);
char *get_path_to_cmd(char *cmd, const char *path_var);
int	save_fds(int *in, int *out);

// fonction dans le dossier utils fichier exec_second_utils.c:
void	restore_fds(int in, int out);
int	run_builtin_in_parent(t_command *cmd, t_env **env);
char	**build_child_envp(t_env *env);
void	child_exec_sequence(t_command *cmd, t_env *env, char *path, int path_owned);


// fonction dans le dossier executor fichier pipes.c:
int execute_pipeline(t_command *head, t_env *env);

// fonction dans le dossier utils.c fichier heredoc_utils.c:
int		buf_n_equal(const char *s1, const char *s2, size_t n);
int	line_buffer_grow(t_linebuf *line_buffer);
int	line_buffer_push_char(t_linebuf *line_buffer, char c);
int	line_is_delim(const char *line, size_t line_len, const char *delim);
int	feed_line_buffer(const char *buf, ssize_t n,
							t_linebuf *line_buffer, int write_file_descriptor, const char *delim);

// fonction dans le dossier utils fichier heredoc_second_utils.c:
void	write_prompt(void);
void	write_line_and_reset(t_linebuf *line_buffer, int write_file_descriptor);
void	heredoc_init(t_linebuf *line_buffer);


// parsing de chat gpt:
int  build_cmd_list_naive(const char *line, t_command **out_head);
void free_cmd_list(t_command *head);
int  execute_command_list(t_command *head, t_env *env);








#endif
