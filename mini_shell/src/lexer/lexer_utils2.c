/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:19:51 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/04 17:11:44 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

/*fonction pour checker les operateurs ensuite les extraire
et les rajouter dans un structure token tout en signalant
via un boolean que c'est bien un operateur*/
static int	handle_input_redir(char *line, t_token *token, int *i)
{
	if (!line || !line[*i])
		return (0);
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			token->type = TOKEN_HEREDOC;
			token->string = ft_strdup("<<");
			*i += 2;
			return (1);
		}
		token->type = TOKEN_REDIR_IN;
		token->string = ft_strdup("<");
		*i += 1;
		return (1);
	}
	return (0);
}

static int	handle_output_redir(char *line, t_token *token, int *i)
{
	if (!line || !line[*i])
		return (0);
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			token->type = TOKEN_REDIR_APPEND;
			token->string = ft_strdup(">>");
			*i += 2;
			return (1);
		}
		token->type = TOKEN_REDIR_OUT;
		token->string = ft_strdup(">");
		*i += 1;
		return (1);
	}
	return (0);
}

static int	handle_pipe(char *line, t_token *token, int *i)
{
	if (!line || !line[*i])
		return (0);
	if (line[*i] == '|')
	{
		token->type = TOKEN_PIPE;
		token->string = ft_strdup("|");
		*i += 1;
		return (1);
	}
	return (0);
}

int	extract_operator(char *line, t_token *token, int *i)
{
	if (handle_input_redir(line, token, i))
		return (1);
	if (handle_output_redir(line, token, i))
		return (1);
	if (handle_pipe(line, token, i))
		return (1);
	return (0);
}
