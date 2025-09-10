/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 15:08:37 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/04 17:08:51 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

t_command	*parse_single_command(t_token **token)
{
	t_command	*new_cmd;
	int			i;

	i = 0;
	new_cmd = init_cmd(nb_args(token));
	while ((*token) && (*token)->type != TOKEN_PIPE)
	{
		if ((*token)->type == TOKEN_WORD)
			parse_word(token, new_cmd, &i);
		else if ((*token)->type == TOKEN_REDIR_IN)
			parse_redir_in(token, new_cmd);
		else if ((*token)->type == TOKEN_HEREDOC)
			parse_heredoc(token, new_cmd);
		else if ((*token)->type == TOKEN_REDIR_OUT)
			parse_redir_out(token, new_cmd);
		else if ((*token)->type == TOKEN_REDIR_APPEND)
			parse_redir_append(token, new_cmd);
	}
	new_cmd->argv[i] = NULL;
	return (new_cmd);
}

t_command	*tokens_to_cmd(t_token **token)
{
	if (!token || !(*token))
		return (NULL);
	return (parse_single_command(token));
}

t_command	*parsing_cmds(t_token **token)
{
	t_command	*cmd_lst;
	t_command	*new_cmd;
	t_command	*last;

	cmd_lst = NULL;
	last = NULL;
	if (!token || !*token)
		return (NULL);
	while (*token)
	{
		new_cmd = tokens_to_cmd(token);
		if (!new_cmd)
			return (NULL);
		if (cmd_lst == NULL)
			cmd_lst = new_cmd;
		else
			last->next = new_cmd;
		last = new_cmd;
		while (*token && (*token)->type != TOKEN_PIPE)
			*token = (*token)->next;
		if (*token && (*token)->type == TOKEN_PIPE)
			*token = (*token)->next;
	}
	return (cmd_lst);
}
