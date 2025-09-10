/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_check2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 16:24:27 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/02 16:58:23 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"
/*fonction pour verifier que les single
quotes sont bien fermé dans la ligne de
commande*/
static int	is_closed_S_quote(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!line)
		return (0);
	while (line[i])
	{
		if (line[i] == '\'')
			count++;
		i++;
	}
	if (count % 2 != 0)
		return (0);
	return (1);
}
/*fonction pour verifier que les double
quotes sont bien fermé dans la ligne de
commande*/
static int	is_closed_D_quote(char *line)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (!line)
		return (0);
	while (line[i])
	{
		if (line[i] == '"')
			count++;
		i++;
	}
	if (count % 2 != 0)
		return (0);
	return (1);
}
/*fonction pour afficher un message d'erreur
en cas de probleme de syntaxe pour indiquer
quel est le probleme detecter*/
static int	error_line(char *message)
{
	ft_putendl_fd(message, 2);
	return (0);
}
/*foction final pour verifier chaques problemes
de syntaxte possible dans la ligne de commande.
Elles fait appel a toutes les autres fonction.*/
int	is_valid_line(char *line, t_token *tokens)
{
	if (!is_closed_S_quote(line))
		return error_line("Syntax error : unclosed single quotes!");
	if (!is_closed_D_quote(line))
		return error_line("Syntax error : unclosed double quotes!");
	if (!is_valid_redirection(tokens))
		return error_line("Syntax error : invalid redirection!");
	if (!consecutive_pipes(tokens))
		return error_line("Syntax error : consecutive pipes!");
	if (!valid_pipe_position(tokens))
		return error_line("Syntax error : invalid pipe position!");
	return (1);
}
