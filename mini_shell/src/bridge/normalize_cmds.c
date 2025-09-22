/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_cmds.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:26:22 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/22 13:26:32 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_redir	*new_redir(t_redir_type type, char *target)
{
	t_redir	*r;

	r = (t_redir *)malloc(sizeof(t_redir));
	if (!r)
		return (NULL);
	r->type = type;
	r->target = ft_strdup(target);
	if (!r->target)
	{
		free(r);
		return (NULL);
	}
	r->fd = -1;
	r->next = NULL;
	return (r);
}

static int	push_redir(t_command *cmd, t_redir_type t, char *s)
{
	t_redir	*node;
	t_redir	*cur;

	node = new_redir(t, s);
	if (!node)
		return (0);
	if (!cmd->redir)
	{
		cmd->redir = node;
		return (1);
	}
	cur = cmd->redir;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
	return (1);
}

int	normalize_parsed_cmds(t_command *head)
{
	t_command	*cur;

	cur = head;
	while (cur)
	{
		if (cur->infile && cur->heredoc == 1)
			if (!push_redir(cur, REDIR_HEREDOC, cur->infile))
				return (0);
		if (cur->infile && cur->heredoc == 0)
			if (!push_redir(cur, REDIR_INPUT, cur->infile))
				return (0);
		if (cur->outfile && cur->append == 1)
			if (!push_redir(cur, REDIR_APPEND, cur->outfile))
				return (0);
		if (cur->outfile && cur->append == 0)
			if (!push_redir(cur, REDIR_OUTPUT, cur->outfile))
				return (0);
		cur = cur->next;
	}
	return (1);
}

void	clear_parser_redir_fields(t_command *head)
{
	t_command	*cur;

	cur = head;
	while (cur)
	{
		cur->infile = NULL;
		cur->outfile = NULL;
		cur = cur->next;
	}
}
