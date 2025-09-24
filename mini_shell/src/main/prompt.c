/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:59:41 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/22 14:59:44 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"
/*
static void	exec_cmds(t_command *cmds, t_env *env)
{
	int	n;

	if (!cmds)
		return ;
	n = count_cmd(cmds);
	if (n > 1)
		execute_pipeline(cmds, env);
	else
		handle_command(cmds, env);
}

static void	process_line(char *line, t_env *env)
{
	t_token		*toks;
	t_command	*cmds;

	if (is_blank_line(line))
		return ;
	toks = convert_line_to_tokens(line, env);
	if (!toks)
	{
		ft_putendl_fd("Lexing failed: invalid input", 2);
		return ;
	}
	if (!is_valid_line(line, toks))
	{
		free_token(toks);
		return ;
	}
	cmds = parsing_cmds(&toks);
	if (!cmds || !normalize_parsed_cmds(cmds))
	{
		free_token(toks);
		return ;
	}
	exec_cmds(cmds, env);
	free_token(toks);
	free_cmd_list(cmds);
}

static int	handle_eof(char *line)
{
	if (!line)
	{
		ft_putendl_fd("exit", 1);
		g_exit_code = 0;
		return (1);
	}
	return (0);
}

void	prompt_loop(char **envp)
{
	char	*line;
	t_env	*env;

	env = init_env(envp);
	setup_prompt_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (handle_eof(line))
			break ;
		if (is_blank_line(line))
		{
			free(line);
			continue ;
		}
		add_history(line);
		process_line(line, env);
		free(line);
	}
	free_env(&env);
}
*/


static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

static char	*substr_dup(char const *s, int start, int len)
{
	char	*out;

	out = (char *)malloc((size_t)len + 1);
	if (!out)
		return (NULL);
	if (len > 0)
		ft_memcpy(out, s + start, (size_t)len);
	out[len] = '\0';
	return (out);
}


static char *read_word(char const *s, int *i)
{
    int     start;
    char    quote;
    char    *piece;
    char    *acc;

    acc = NULL;
    while (s[*i] && !is_space(s[*i]) && s[*i] != '|' && s[*i] != '<' && s[*i] != '>')
    {
        if (s[*i] == '\'' || s[*i] == '"')
        {
            quote = s[*i];
            (*i)++;
            start = *i;
            while (s[*i] && s[*i] != quote)
                (*i)++;
            if (!s[*i])
                break ;
            piece = substr_dup(s, start, *i - start);
            if (!piece)
                return (free(acc), NULL);
            if (!acc)
                acc = piece;
            else
            {
                char *tmp = ft_strjoin(acc, piece);
                free(acc);
                free(piece);
                if (!tmp)
                    return (NULL);
                acc = tmp;
            }
            (*i)++;
        }
        else
        {
            start = *i;
            while (s[*i] && !is_space(s[*i]) && s[*i] != '|' && s[*i] != '<' && s[*i] != '>')
                (*i)++;
            piece = substr_dup(s, start, *i - start);
            if (!piece)
                return (free(acc), NULL);
            if (!acc)
                acc = piece;
            else
            {
                char *tmp = ft_strjoin(acc, piece);
                free(acc);
                free(piece);
                if (!tmp)
                    return (NULL);
                acc = tmp;
            }
        }
    }
    if (!acc)
        acc = substr_dup("", 0, 0);
    return (acc);
}



typedef struct s_strnode
{
	char				*s;
	struct s_strnode	*next;
}	t_strnode;

static void	strlist_push(t_strnode **head, t_strnode **tail, char *s, int *count)
{
	t_strnode *n;

	n = (t_strnode *)malloc(sizeof(*n));
	if (!n)
	{
		free(s);
		return ;
	}
	n->s = s;
	n->next = NULL;
	if (!*head)
		*head = n;
	else
		(*tail)->next = n;
	*tail = n;
	(*count)++;
}

static t_command	*temp_new_cmd(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = (char **)malloc(sizeof(char *));
	if (!cmd->argv)
		return (free(cmd), NULL);
	cmd->argv[0] = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc = 0;
	cmd->append = 0;
	cmd->redir = NULL;
	cmd->next = NULL;
	return (cmd);
}

static int	finalize_argv_from_list(t_command *cmd, t_strnode *head, int count)
{
	char		**v;
	int			i;
	t_strnode	*cur;
	t_strnode	*nx;

	v = (char **)malloc(sizeof(char *) * (count + 1));
	if (!v)
		return (0);
	i = 0;
	cur = head;
	while (cur)
	{
		v[i++] = cur->s;
		nx = cur->next;
		free(cur);
		cur = nx;
	}
	v[i] = NULL;
	free(cmd->argv);
	cmd->argv = v;
	return (1);
}


static char const *peek_op(char const *s, int i)
{
	if (!s[i])
		return (NULL);
	if (s[i] == '|')
		return ("|");
	if (s[i] == '<' && s[i + 1] == '<')
		return ("<<");
	if (s[i] == '>' && s[i + 1] == '>')
		return (">>");
	if (s[i] == '<')
		return ("<");
	if (s[i] == '>')
		return (">");
	return (NULL);
}

static t_command	*parse_line_minimal(char const *line)
{
	int			i;
	t_command	*head;
	t_command	*cur_cmd;
	t_command	*last;
	t_strnode	*args_head;
	t_strnode	*args_tail;
	int			arg_count;

	i = 0;
	head = NULL;
	last = NULL;
	cur_cmd = temp_new_cmd();
	if (!cur_cmd)
		return (NULL);
	args_head = NULL;
	args_tail = NULL;
	arg_count = 0;
	while (line[i])
	{
		while (line[i] && is_space(line[i]))
			i++;
		if (!line[i])
			break ;
		if (peek_op(line, i) == NULL)
		{
			char *w = read_word(line, &i);
			if (!w)
				return (free_cmd_list(cur_cmd), NULL);
			strlist_push(&args_head, &args_tail, w, &arg_count);
		}
		else if (line[i] == '|')
		{
			if (!finalize_argv_from_list(cur_cmd, args_head, arg_count))
				return (free_cmd_list(cur_cmd), NULL);
			args_head = NULL; args_tail = NULL; arg_count = 0;
			if (!head)
				head = cur_cmd;
			else
				last->next = cur_cmd;
			last = cur_cmd;
			cur_cmd = temp_new_cmd();
			if (!cur_cmd)
				return (free_cmd_list(head), NULL);
			i++;
		}
		else if (line[i] == '<' && line[i + 1] == '<')
		{
			char *delim;

			i += 2;
			while (line[i] && is_space(line[i]))
				i++;
			if (!line[i])
				return (free_cmd_list(cur_cmd), NULL);
			delim = read_word(line, &i);
			if (!delim)
				return (free_cmd_list(cur_cmd), NULL);
			if (cur_cmd->infile)
				free(cur_cmd->infile);
			cur_cmd->infile = delim;
			cur_cmd->heredoc = 1;
		}
		else if (line[i] == '<')
		{
			char *infile;

			i++;
			while (line[i] && is_space(line[i]))
				i++;
			if (!line[i])
				return (free_cmd_list(cur_cmd), NULL);
			infile = read_word(line, &i);
			if (!infile)
				return (free_cmd_list(cur_cmd), NULL);
			if (cur_cmd->infile)
				free(cur_cmd->infile);
			cur_cmd->infile = infile;
			cur_cmd->heredoc = 0;
		}
		else if (line[i] == '>' && line[i + 1] == '>')
		{
			char *outfile;

			i += 2;
			while (line[i] && is_space(line[i]))
				i++;
			if (!line[i])
				return (free_cmd_list(cur_cmd), NULL);
			outfile = read_word(line, &i);
			if (!outfile)
				return (free_cmd_list(cur_cmd), NULL);
			if (cur_cmd->outfile)
				free(cur_cmd->outfile);
			cur_cmd->outfile = outfile;
			cur_cmd->append = 1;
		}
		else if (line[i] == '>')
		{
			char *outfile;

			i++;
			while (line[i] && is_space(line[i]))
				i++;
			if (!line[i])
				return (free_cmd_list(cur_cmd), NULL);
			outfile = read_word(line, &i);
			if (!outfile)
				return (free_cmd_list(cur_cmd), NULL);
			if (cur_cmd->outfile)
				free(cur_cmd->outfile);
			cur_cmd->outfile = outfile;
			cur_cmd->append = 0;
		}
	}

	if (!finalize_argv_from_list(cur_cmd, args_head, arg_count))
		return (free_cmd_list(cur_cmd), NULL);
	if (!head)
		head = cur_cmd;
	else
		last->next = cur_cmd;
	return (head);
}


static int	handle_eof_local(char *line)
{
	if (!line)
	{
		ft_putendl_fd("exit", 1);
		return (1);
	}
	return (0);
}

static void	process_line_miniparser(char *line, t_env *env)
{
	t_command	*cmds;

	cmds = parse_line_minimal(line);
	if (!cmds)
		return ;
	if (!normalize_parsed_cmds(cmds))
	{
		free_cmd_list(cmds);
		return ;
	}
	clear_parser_redir_fields(cmds);
	handle_command(cmds, env);
	free_cmd_list(cmds);
}

void	prompt_loop(char **envp)
{
	char	*line;
	t_env	*env;

	env = init_env(envp);
	setup_prompt_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (handle_eof_local(line))
			break ;
		if (is_blank_line(line))
		{
			free(line);
			continue ;
		}
		add_history(line);
		process_line_miniparser(line, env);
		free(line);
	}
	free_env(&env);
}

