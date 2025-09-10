/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/15 17:38:36 by kingstephan       #+#    #+#             */
/*   Updated: 2025/08/20 01:46:03 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

static const char	*get_type_char(int type)
{
	if (type == TOKEN_WORD)
		return("WORD");
	else if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	else if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	else if (type == TOKEN_REDIR_APPEND)
		return ("REDIR_APPEND");
	else if (type == TOKEN_PIPE)
		return ("PIPE");
	else
		return ("UNKNOWN");
}

void	print_tokens(t_token *head)
{
	while (head)
	{
		printf("[%s] : %s\n", get_type_char(head->type), head->string);
		head = head->next;
	}
}

void	print_cmd(t_command *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
	{
		printf("Commande vide.\n");
		return;
	}
	printf("cmd :\n");
	while (cmd->argv[i])
	{
		printf("args = [%s]\n", cmd->argv[i]);
		i++;
	}
	printf("infile      =      [%s]\n", cmd->infile ? cmd->infile : "NULL");
	printf("outfile      =      [%s]\n", cmd->outfile ? cmd->outfile : "NULL");
	printf("append      =      [%d]\n", cmd->append);
	printf("heredoc      =      [%d]\n", cmd->heredoc);
}

void	print_all_cmds(t_command *cmd_list)
{
	int	i;

	i = 1;
	while (cmd_list)
	{
		printf("===== command %d =====\n", i++);
		print_cmd(cmd_list);
		cmd_list = cmd_list->next;
	}
}
