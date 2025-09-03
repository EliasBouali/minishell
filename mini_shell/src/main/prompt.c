/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:44:30 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:44:31 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// parsing fait par chat le tant que Stephane finisse son parsing
#include "../include/minishell.h"
#include <string.h> // memset

/* ========== Helpers internes ========== */

static char	*ft_itoa(int n)
{
	char	buf[12];
	int		len;
	int		tmp;
	char	*res;
	int		i;

	if (n == 0)
		return (ft_strdup("0"));
	if (n < 0) // sécurité, mais $? sera toujours >=0
		n = -n;
	len = 0;
	tmp = n;
	while (tmp > 0)
	{
		buf[len++] = '0' + (tmp % 10);
		tmp /= 10;
	}
	res = (char *)malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = buf[len - 1 - i];
		i++;
	}
	res[len] = '\0';
	return (res);
}

static void	free_redir(t_redir **head)
{
	t_redir	*redirection;
	t_redir	*tmp;

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

/* ========== Parsing naïf (temporaire) ========== */

static void	parse_line_naive(const char *line, t_command *cmd)
{
	char	**tokens;
	t_redir	*r_head;
	char	**argv_tmp;
	int		argc_tmp;
	t_redir	*r;
	char	*num;
	t_redir	*x;

	r_head = NULL, *r_tail;
	tokens = NULL;
	r_head = NULL, r_tail = NULL;
	argv_tmp = NULL;
	argc_tmp = 0;
	int i, j;
	cmd->args = NULL;
	cmd->redir = NULL;
	tokens = ft_split(line, ' ');
	if (!tokens)
		return ;
	/* 1) Compter les tokens non-redirections */
	i = 0;
	while (tokens[i])
	{
		if (!ft_strncmp(tokens[i], ">>", 3) || !ft_strncmp(tokens[i], "<<", 3)
			|| !ft_strncmp(tokens[i], ">", 2) || !ft_strncmp(tokens[i], "<", 2))
		{
			if (!tokens[i + 1])
			{
				ft_putendl_fd("minishell: syntax error near unexpected token",
					2);
				break ;
			}
			i += 2;
			continue ;
		}
		argc_tmp++;
		i++;
	}
	if (argc_tmp > 0)
	{
		argv_tmp = (char **)malloc(sizeof(char *) * (argc_tmp + 1));
		if (!argv_tmp)
		{
			ft_putendl_fd("minishell: alloc error", 2);
			free_split(tokens);
			return ;
		}
	}
	/* 2) Construire argv_tmp et la liste des redirections */
	j = 0;
	i = 0;
	while (tokens[i])
	{
		r = NULL;
		if (!ft_strncmp(tokens[i], ">>", 3) && tokens[i + 1])
		{
			r = (t_redir *)malloc(sizeof(*r));
			if (!r)
			{
				ft_putendl_fd("minishell: alloc error", 2);
				goto fail;
			}
			r->type = REDIR_APPEND;
			r->target = ft_strdup(tokens[i + 1]);
			r->fd = -1;
			r->next = NULL;
		}
		else if (!ft_strncmp(tokens[i], "<<", 3) && tokens[i + 1])
		{
			r = (t_redir *)malloc(sizeof(*r));
			if (!r)
			{
				ft_putendl_fd("minishell: alloc error", 2);
				goto fail;
			}
			r->type = REDIR_HEREDOC;
			r->target = ft_strdup(tokens[i + 1]);
			r->fd = -1;
			r->next = NULL;
		}
		else if (!ft_strncmp(tokens[i], ">", 2) && tokens[i + 1])
		{
			r = (t_redir *)malloc(sizeof(*r));
			if (!r)
			{
				ft_putendl_fd("minishell: alloc error", 2);
				goto fail;
			}
			r->type = REDIR_OUTPUT;
			r->target = ft_strdup(tokens[i + 1]);
			r->fd = -1;
			r->next = NULL;
		}
		else if (!ft_strncmp(tokens[i], "<", 2) && tokens[i + 1])
		{
			r = (t_redir *)malloc(sizeof(*r));
			if (!r)
			{
				ft_putendl_fd("minishell: alloc error", 2);
				goto fail;
			}
			r->type = REDIR_INPUT;
			r->target = ft_strdup(tokens[i + 1]);
			r->fd = -1;
			r->next = NULL;
		}
		else
		{
			/* token normal → copie; expansion minimale de "$?" */
			if (argv_tmp)
			{
				if (!ft_strncmp(tokens[i], "$?", 3))
				{
					num = ft_itoa(g_exit_code);
					argv_tmp[j++] = num ? num : ft_strdup("$?");
				}
				else
					argv_tmp[j++] = ft_strdup(tokens[i]);
			}
			i++;
			continue ;
		}
		if (!r || !r->target)
		{
			ft_putendl_fd("minishell: alloc error", 2);
			if (r)
				free(r);
			goto fail;
		}
		if (!r_head)
			r_head = r;
		else
			r_tail->next = r;
		r_tail = r;
		i += 2;
	}
	if (argv_tmp)
		argv_tmp[j] = NULL;
	free_split(tokens);
	cmd->args = argv_tmp;
	cmd->redir = r_head;
	return ;
fail:
	if (argv_tmp)
	{
		while (j-- > 0)
			free(argv_tmp[j]);
		free(argv_tmp);
	}
	{
		x = r_head, *tmp;
		while (x)
		{
			tmp = x->next;
			if (x->target)
				free(x->target);
			free(x);
			x = tmp;
		}
	}
	free_split(tokens);
	cmd->args = NULL;
	cmd->redir = NULL;
}

/* ========== Gestion liste commandes (naïf) ========== */

int	build_cmd_list_naive(const char *line, t_command **out_head)
{
	char	**parts;
	int		i;

	t_command *head, *tail, *node;
	*out_head = NULL;
	parts = ft_split(line, '|'); // naïf: pas de quotes/escapes
	if (!parts)
		return (-1);
	head = NULL;
	tail = NULL;
	i = 0;
	while (parts[i])
	{
		node = (t_command *)malloc(sizeof(t_command));
		if (!node)
		{
			free_split(parts);
			free_cmd_list(head);
			return (-1);
		}
		memset(node, 0, sizeof(*node));
		parse_line_naive(parts[i], node);
		node->next = NULL;
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		i++;
	}
	free_split(parts);
	*out_head = head;
	return (0);
}

void	free_cmd_list(t_command *head)
{
	t_command	*next;

	while (head)
	{
		next = head->next;
		if (head->args)
			free_split(head->args);
		if (head->redir)
			free_redir(&head->redir);
		free(head);
		head = next;
	}
}

int	execute_command_list(t_command *head, t_env *env)
{
	int	n;

	if (!head)
		return (g_exit_code);
	n = count_cmd(head); // <- prototype dans minishell.h
	if (n <= 1)
	{
		handle_command(head, env); // ta fonction existante
		return (g_exit_code);
	}
	return (execute_pipeline(head, env)); // défini dans pipes.c
}

static int	handle_eof(char *line)
{
	if (line)
		return (0);
	ft_putendl_fd("exit", 1);
	g_exit_code = 0;
	return (1);
}

static void	process_line(char *line, t_env *env)
{
	t_command	*head;

	if (!line)
		return ;
	if (is_blank_line(line))
	{
		free(line);
		return ;
	}
	add_history(line);
	head = NULL;
	if (build_cmd_list_naive(line, &head) == 0)
	{
		execute_command_list(head, env);
		free_cmd_list(head);
	}
	free(line);
}

void	prompt_loop(char **envp)
{
	char	*line;
	t_env	*env;

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
		if (handle_eof(line))
			break ;
		process_line(line, env);
	}
	free_env(&env);
}
