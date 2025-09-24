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

/*fonction pour verifier que les quotes sont bien fermé*/
static int	check_quotes(char *line)
{
	int	in_double;
	int	in_single;
	int	i;

	if(!line)
		return (0);
	in_double = 0;
	in_single = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' && in_double == 0)
			in_single = 1 - in_single;
		if (line[i] == '"' && in_single == 0)
			in_double = 1 - in_double;
		i++;
	}
	if (in_double == 0 && in_single == 0)
		return (1);
	return (0);
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
	if (!tokens)
		return (1);
	if(!check_quotes(line))
		return (error_line("Syntax error : quotes not closed!"));
	if (!is_valid_redirection(tokens))
		return (error_line("Syntax error : invalid redirection!"));
	if (!consecutive_pipes(tokens))
		return (error_line("Syntax error : consecutive pipes!"));
	if (!valid_pipe_position(tokens))
		return (error_line("Syntax error : invalid pipe position!"));
	if (!consecutive_redirections(tokens))
		return (error_line("Syntax error : consecutive redirection!"));
	return (1);
}
