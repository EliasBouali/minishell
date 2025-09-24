/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:43:00 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/04 17:23:22 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_token		t_token;
typedef struct s_command	t_command;

/*---------------LEXER_UTILS.C---------------*/
int							is_white_space(char c);
int							is_operator(char c);
int							is_quoted(char c);
void						skip_white_space(char *line, int *i);
/*---------------LEXER_UTILS1.C---------------*/
char						*extract_d_quoted(char *line, int *i);
char						*extract_s_quoted(char *line, int *i);
char						*extract_unquoted(char *line, int *i);
/*---------------LEXER_UTILS2.C---------------*/
// int			handle_input_redir(char *line, t_token *token, int *i);
// int			handle_output_redir(char *line, t_token *token, int *i);
// int			handle_pipe(char *line, t_token *token, int *i);
int							extract_operator(char *line, t_token *token,
								int *i);
/*---------------LEXER_UTILS3.C---------------*/
// int			append_s_quoted(char *line, int *i, char **final_word);
// int			append_d_quoted(char *line, int *i, char **final_word);
// int			append_unquoted(char *line, int *i, char **final_word);
char						*collect_word(char *line, int *i, t_env *env);
/*---------------LEXER_UTILS4.C---------------*/
int							var_start_ok(char c);
int							check_special_dollar_case(char *str, int i);
char						*extract_var_name(char *str, int *i);
char						*get_var_value(char *name, t_env *env);
int							get_var_len(char *str, int *i, t_env *env);
/*---------------LEXER_UTILS5.C---------------*/
void						free_cmd_list(t_command *cmd);
int							calculate_total_size(char *str, t_env *env);
int             add_var_value(char *str, char *result, int *i, t_env *env);
char						*expand_variables(char *str, t_env *env);
/*---------------LEXER_UTILS6.C---------------*/
t_token						*new_token(char *string, int type);
void						add_token(t_token **head, t_token *new_token);
void						free_token(t_token *head);
int							emit_token(t_token **head, char *str, int type);
/*---------------LEXER.C---------------*/
t_token						*convert_line_to_tokens(char *line, t_env *env);
/*---------------LINE_CHECK1/2.C---------------*/
int							is_valid_redirection(t_token *tokens);
int							consecutive_pipes(t_token *tokens);
int							consecutive_redirections(t_token *tokens);
int							valid_pipe_position(t_token *tokens);
int							is_valid_line(char *line, t_token *tokens);
/*---------------PARSER_UTILS.C---------------*/
int							nb_args(t_token **token);
t_command					*init_cmd(int word_count);
char						*safe_ft_strdup(char *s, t_command **cmd_lst);
void						clean_cmd_argv(char **argv, int count);
int							is_redirection(t_token *token);
/*---------------PARSER_UTILS1.C---------------*/
int							skip_redirection_token(t_token **token);
int							add_word_to_argv(t_token **token, t_command *cmd,
								int *i);
/*---------------PARSER_UTILS1.C---------------*/
int							parse_word(t_token **tokens, t_command *cmd,
								int *i);
int							parse_redir_in(t_token **tokens, t_command *cmd);
int							parse_heredoc(t_token **tokens, t_command *cmd);
int							parse_redir_out(t_token **tokens, t_command *cmd);
int							parse_redir_append(t_token **tokens,
								t_command *cmd);
/*---------------PARSER.C---------------*/
t_command					*parse_single_command(t_token **token);
t_command					*tokens_to_cmd(t_token **token);
t_command					*parsing_cmds(t_token **token);
/*---------------PRINT.C---------------*/
void						print_tokens(t_token *head);
void						print_cmd(t_command *cmd);
void						print_all_cmds(t_command *cmd_list);

/*echo "bonjour $USER dans $HOME" < infile.txt | grep
	-i "bonjour" | sort | uniq > sorted.txt*/

#endif
