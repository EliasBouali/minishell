/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_check1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 13:39:48 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/02 17:07:25 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

/*fichier pour verifier que la ligne de commande
est valide dans sa syntaxe. toutes ces fonctions
seront ensuite appeler dans le fichier
line_check2.c dans la fonction is_valid_line
qui va verifier toutes les erreurs de syntaxes*/
/*fonction pour verifier qu'il ya bien un fichier
apres la redirection*/
int	is_valid_redirection(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (0);
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_REDIR_OUT)
		{
			if (current->next == NULL || current->next->type != TOKEN_WORD)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

/*fonction pour verifier qu'il n'y a pas deux
pipes qui se suivent*/
int	consecutive_pipes(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (0);
	current = tokens;
	while (current && current->next)
	{
		if (current->type == TOKEN_PIPE)
			if (current->next->type == TOKEN_PIPE)
				return (0);
		current = current->next;
	}
	return (1);
}

/*fonction pour verifier qu'il n'y a pas
deux redirection qui se suivent*/
int	consecutive_redirections(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (0);
	current = tokens;
	while (current && current->next)
	{
		if (current->type == TOKEN_HEREDOC
			|| current->type == TOKEN_REDIR_APPEND
			|| current->type == TOKEN_REDIR_IN
			|| current->type == TOKEN_REDIR_OUT)
		{
			if (current->next->type == TOKEN_HEREDOC
				|| current->next->type == TOKEN_REDIR_APPEND
				|| current->next->type == TOKEN_REDIR_IN
				|| current->next->type == TOKEN_REDIR_OUT)
				return (0);
		}
		current = current->next;
	}
	return (1);
}

/*fonction pour verifier que le pipe est à la
bonne position. Donc pas au debut ni à la fin*/
int	valid_pipe_position(t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (0);
	current = tokens;
	if (current->type == TOKEN_PIPE)
		return (0);
	while (current)
	{
		if (current->type == TOKEN_PIPE && current->next == NULL)
			return (0);
		current = current->next;
	}
	return (1);
}
