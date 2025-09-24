/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:52:44 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/04 18:16:28 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

int	parse_word(t_token **token, t_command *cmd, int *i)
{
	cmd->argv[(*i)] = ft_strdup((*token)->string);
	if (!cmd->argv[(*i)])
		return (0);
	(*i)++;
	*token = (*token)->next;
	return (1);
}

int	parse_redir_in(t_token **token, t_command *cmd)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup((*token)->next->string);
	if (!cmd->infile)
		return (0);
	cmd->heredoc = 0;
	*token = (*token)->next->next;
	return (1);
}

int	parse_heredoc(t_token **token, t_command *cmd)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup((*token)->next->string);
	if (!cmd->infile)
		return (0);
	cmd->heredoc = 1;
	*token = (*token)->next->next;
	return (1);
}

int	parse_redir_out(t_token **token, t_command *cmd)
{
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup((*token)->next->string);
	if (!cmd->outfile)
		return(0);
	cmd->append = 0;
	*token = (*token)->next->next;
	return (1);
}
int	parse_redir_append(t_token **token, t_command *cmd)
{
	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = ft_strdup((*token)->next->string);
	if (!cmd->outfile)
		return(0);
	cmd->append = 1;
	*token = (*token)->next->next;
	return (1);
}
