/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 01:04:14 by kingstephan       #+#    #+#             */
/*   Updated: 2025/08/27 18:15:49 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"
/*fonction pour verifier si c'est un espace blanc*/
int	is_white_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}
/*fonction pour verifier si on a un operateur*/
int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}
/*fonction pour verifier les double et single quote*/
int	is_quoted(char c)
{
	if (c == '"' || c == '\'')
		return (1);
	return (0);
}
/*fonction qui saute les espaces blancs*/
void	skip_white_space(char *line, int *i)
{
	if (!line || !line[*i])
		return ;
	while (line[*i] && is_white_space(line[*i]))
		(*i)++;
}
