/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:59:36 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/02 19:08:06 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

/*fonction qui saute la redirection pour prendre
le nom du fichier qui vient juste apres la redirection.
ca evite de mettre la redirection dans args*/
int	skip_redirection_token(t_token **token)
{
	if (!token || !*token || !(*token)->next)
		return (0);
	if (!is_redirection(*token))
		return (0);
	*token = (*token)->next;
	if (*token)
	{
		*token = (*token)->next;
	}
	else
		return (0);
	return (1);
}
/*ma fonction qui va remplir les argv de toute ma commande*/

int	add_word_to_argv(t_token **token, t_command *cmd, int *i)
{
	if (!token || !(*token) || (*token)->type != TOKEN_WORD || !cmd)
		return (0);
	cmd->argv[*i] = safe_ft_strdup((*token)->string, NULL);
	if (!cmd->argv[*i])
	{
		clean_cmd_argv(cmd->argv, *i);
		return (0);
	}
	(*i)++;
	*token = (*token)->next;
	return (1);
}
