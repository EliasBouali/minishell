/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 16:02:34 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/07 16:02:42 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ===== INCLUDES (stdio AVANT readline) ===== */
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_env	t_env;

/* ===== GLOBAUX ===== */
extern int				g_exit_code;

/* table/helpers (si utilisés par builtins.c) */
int						cmd_is_builtin(char const *cmd);
int						exec_builtin(char **argv, t_env **env);

/* ===== ENV ===== */
typedef struct s_env
{
	char				*value;
	char				*name;
	struct s_env		*next;
}						t_env;

/* ===== TOKENS (interfaces lexer) ===== */
typedef struct s_token
{
	int					type;
	char				*string;
	struct s_token		*next;
}						t_token;

/* Garder EXACTEMENT ces noms pour matcher parser/line_checker */
enum					e_token_type
{
	TOKEN_WORD = 0,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC
};

/* ===== REDIRS (attendues par l'exécuteur) ===== */
typedef enum e_redir_type
{
	REDIR_INPUT = 0,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}						t_redir_type;

typedef struct s_redir
{
	t_redir_type		type;
	char				*target;
	int					fd;
	struct s_redir		*next;
}						t_redir;

/* ===== COMMAND UNIFIÉE =====
   - parser remplit: argv, infile, outfile, heredoc, append, next
   - bridge remplit: redir (liste)
*/
typedef struct s_command
{
	char				**argv;
	char				*infile;
	char				*outfile;
	int					heredoc;
	int					append;
	t_redir				*redir;
	struct s_command	*next;
}						t_command;

# ifndef MSH_NO_COMPAT
#  define ARGS argv
# endif

/* ===== HEREDOC BUFFERS (déjà utilisés par ton exécuteur) ===== */
typedef struct s_linebuf
{
	char				*buf;
	size_t				len;
	size_t				cap;
}						t_linebuf;

typedef struct s_feed_ctx
{
	t_linebuf			*line_buffer;
	int					write_fd;
	char const			*delim;
}						t_feed_ctx;

# define HD_INIT_CAP 64

/* ===== PIPELINE CTX (existant côté executor) ===== */
typedef struct s_pipeline_ctx
{
	int					**pipes;
	int					n;
	t_env				*env;
	t_command			*head;
}						t_pipeline_ctx;

/* ===== BUILTINS ===== */
typedef struct s_builtin
{
	const char			*name;
	int					(*function)(char **argv, t_env **env);
}						t_builtin;

int						ft_echo(char **argv, t_env **env);
int						ft_pwd(char **argv, t_env **env);
int						ft_env(char **argv, t_env **env);
int						ft_cd(char **argv, t_env **env);
int						ft_export(char **argv, t_env **env);
int						ft_unset(char **argv, t_env **env);
int						ft_exit(char **argv, t_env **env);
void					free_cmd_list(t_command *head);

/* ===== LEXER/PARSER/CHECKER (du binôme) ===== */
t_token					*convert_line_to_tokens(char *line);
void					free_token(t_token *head);
int						is_valid_line(char *line, t_token *tokens);
t_command				*parsing_cmds(t_token **token);

/* ===== BRIDGE (nouveau) ===== */
int						normalize_parsed_cmds(t_command *head);
void					clear_parser_redir_fields(t_command *head);

/* ===== EXECUTEUR (tes fichiers) ===== */
void					handle_command(t_command *cmd, t_env *env);
int						execute_pipeline(t_command *head, t_env *env);
int						apply_redirections(t_command *cmd, t_env **env);
int						open_heredoc(char const *delim, int *out_fd,
							t_env **env);
void					restore_signals_for_child(void);
int						count_cmd(t_command *head);

/* ===== ENV (tes fichiers) ===== */
t_env					*init_env(char **envp_src);
void					free_env(t_env **env);
char const				*env_get(t_env *env, char const *name);
int						env_set(t_env **env, char const *name,
							char const *value);
int						env_unset(t_env **env, char const *name);
char					**env_to_envp(t_env *env);
void					free_envp_array(char **arr);
t_env					*env_node_new(char const *name, char const *value);
char					**alloc_envp(size_t n);

/* utils/utils.c */
int						ft_strncmp(char const *s1, char const *s2, size_t n);
size_t					ft_strlen(char const *s);
void					free_split(char **split);
char					*ft_strchr(char const *s, int c);
void					ft_putstr_fd(char *s, int fd);
char					*ft_strdup(char const *s);
int						num_count(int n);
void					itoa_fill(char *str, long nb, int index);

/* utils/second_utils.c */
void					ft_putendl_fd(char *s, int fd);
int						is_blank_line(char *s);

/* ===== UTILS (tes fichiers) ===== */
size_t					ft_strlen(char const *s);
int						ft_strncmp(char const *s1, char const *s2, size_t n);
char					*ft_strdup(char const *s);
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strchr(char const *s, int c);
void					ft_putstr_fd(char *s, int fd);
void					ft_putendl_fd(char *s, int fd);
void					free_split(char **split);
long long				ft_atoll(char const *str);
char					**ft_split(char const *s, char c);
char					*ft_strndup(const char *s, size_t end);
char					*ft_strjoin_and_free(char *s1, char *s2);
char					*ft_itoa(int n);
int						ft_isalnum(int c);
int						ft_isdigit(int c);
int						ft_isalpha(int c);

/* utils/exec_second_utils.c */
void					restore_fds(int in, int out);
int						run_builtin_in_parent(t_command *cmd, t_env **env);
char					**build_child_envp(t_env *env);
void					child_exec_sequence(t_command *cmd, t_env *env,
							char *path, int path_owned);
int						check_slash_cmd_fs(char const *name);

/* utils/exec_third_utils.c */
char					*cmd_not_found(char *name);
char					*alloc_fail(void);
char					*path_from_dirs(char const *name, char **dirs,
							int *owned);

/* executor/pipes.c */
int						execute_pipeline(t_command *head, t_env *env);

/* utils/heredoc_utils.c */
int						buf_n_equal(char const *s1, char const *s2, size_t n);
int						line_buffer_grow(t_linebuf *line_buffer);
int						line_buffer_push_char(t_linebuf *line_buffer, char c);
int						line_is_delim(char const *line, size_t line_len,
							char const *delim);
int						feed_line_buffer(char const *buf, ssize_t n,
							t_feed_ctx *ctx);

/* utils/heredoc_second_utils.c */
void					write_prompt(void);
void					write_line_and_reset(t_linebuf *line_buffer,
							int write_file_descriptor);
void					heredoc_init(t_linebuf *line_buffer);

/* utils/pipes_utils.c */
void					restore_signals_for_child(void);
int						count_cmd(t_command *head);
int						**alloc_pipes(int number_of_cmd);
void					free_pipes(int **pipes, int number_of_cmd);
void					close_all_pipes(int **pipes, int number_of_cmd);

/* utils/pipes_second_utils.c */
int						create_pipes(int **pipes, int number_of_cmd);
void					connect_pipes_for_child(int cmd_index,
							int number_of_cmd, int **pipes);
t_command				*get_cmd_position(t_command *head,
							int idx_cmd_pipeline);
void					print_cmd_not_found(char const *name);
int						path_checks_for_slash_cmd(char const *cmd_name);

/* utils/pipes_third_utils.c */
int						wait_pipeline(pid_t *pids, int n);
int						resolve_slash_command(char const *name, char **out_path,
							int *path_owned);
int						resolve_in_path(char const *name, t_env *env,
							char **out_path, int *path_owned);
int						resolve_command(char const *name, t_env *env,
							char **out_path, int *path_owned);
void					exec_external_child(char *path, char **argv, t_env *env,
							int own);

/* utils/pipes_four_utils.c */
int						setup_pipes_and_pids(int n, int ***pipes_out,
							pid_t **pids_out);
int						handle_spawn_error(int i, int n, pid_t *pids,
							int **pipes);

/* utils/exec_utils.c */
void					print_error(char const *name, char const *msg);
/* char	*join_dir_cmd(char *dir, char *cmd); */
char					*get_valid_exec_path(char **path_split, char *cmd);
char					*get_path_to_cmd(char *cmd, char const *path_var);
int						save_fds(int *in, int *out);

/* utils/three_utils.c */
int						is_valid_name(char const *s);
t_env					*find_by_name(t_env *env, char const *name);
void					free_envp_array(char **arr);
void					*ft_memcpy(void *dst, void const *src, size_t n);
long long				ft_atoll(char const *str);

/* ===== PROMPT / SIGNaux ===== */
void					prompt_loop(char **env);
void					setup_prompt_signals(void);
void					prompt_signal_handler(int sign);

#endif
