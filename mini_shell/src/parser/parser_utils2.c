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

void	parse_word(t_token **token, t_command *cmd, int *i)
{
	cmd->argv[(*i)++] = ft_strdup((*token)->string);
	*token = (*token)->next;
}

void	parse_redir_in(t_token **token, t_command *cmd)
{
	cmd->infile = ft_strdup((*token)->next->string);
	cmd->heredoc = 0;
	*token = (*token)->next->next;
}

void	parse_heredoc(t_token **token, t_command *cmd)
{
	cmd->infile = ft_strdup((*token)->next->string);
	cmd->heredoc = 1;
	*token = (*token)->next->next;
}

void	parse_redir_out(t_token **token, t_command *cmd)
{
	cmd->outfile = ft_strdup((*token)->next->string);
	cmd->append = 0;
	*token = (*token)->next->next;
}
void	parse_redir_append(t_token **token, t_command *cmd)
{
	cmd->outfile = ft_strdup((*token)->next->string);
	cmd->append = 1;
	*token = (*token)->next->next;
}
