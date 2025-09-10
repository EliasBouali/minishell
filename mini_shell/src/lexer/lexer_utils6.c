/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 15:48:48 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/04 15:49:28 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"
/*fonction qui cree un nouveau token
ajouter une string et lui donne son type*/
t_token	*new_token(char *string, int type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->string = ft_strdup(string);
	new_token->type = type;
	new_token->next = NULL;
	return (new_token);
}
/*fonction qui ajoute un token a la liste chainé*/
void	add_token(t_token **head, t_token *new_token)
{
	t_token	*last;

	if (*head == NULL)
	{
		*head = new_token;
		return ;
	}
	last = *head;
	while (last->next)
		last = last->next;
	last->next = new_token;
}
/*fonction qui libere ma liste chainé*/
void	free_token(t_token *head)
{
	t_token	*tmp;

	if (head == NULL)
		return ;
	while (head)
	{
		tmp = head->next;
		free(head->string);
		free(head);
		head = tmp;
	}
}
/*fonction combine la creation du token et l'ajout a la liste chainé*/
int	emit_token(t_token **head, char *str, int type)
{
	t_token	*new;

	new = new_token(str, type);
	if (!new)
		return (0);
	add_token(head, new);
	return (1);
}
