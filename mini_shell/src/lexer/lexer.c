/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 19:51:22 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/06 03:55:08 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

static int	handle_operator_token(char *line, int *i, t_token **tokens)
{
	t_token	token_tmp;

	token_tmp.string = NULL;
	if (!line || !tokens || !i)
		return (0);
	if (!extract_operator(line, &token_tmp, i))
	{
		free_token(*tokens);
		return (0);
	}
	if (!emit_token(tokens, token_tmp.string, token_tmp.type))
	{
		free(token_tmp.string);
		free_token(*tokens);
		return (0);
	}
	free(token_tmp.string);
	return (1);
}
static int	handle_word_token(char *line, int *i, t_token **tokens, t_env *env)
{
	char	*word;

	if (!line || !tokens || !i)
		return (0);
	word = collect_word(line, i, env);
	if (!word)
	{
		free_token(*tokens);
		*tokens = NULL;
		return (0);
	}
	if (!emit_token(tokens, word, TOKEN_WORD))
	{
		free(word);
		free_token(*tokens);
		*tokens = NULL;
		return (0);
	}
	free(word);
	return (1);
}
/*fonction final qui va regarder dans la ligne de commande
sauter les espaces blanc, trouver quels type de mots on a
l'ajouter dans la liste chainé avec son mots et son type
et continuer jusqu'a la fin de la ligne de commande*/
t_token	*convert_line_to_tokens(char *line, t_env *env)
{
	int		i;
	t_token	*tokens;
	t_token	token_tmp;

	i = 0;
	tokens = NULL;
	token_tmp.string = NULL;
	while (line[i])
	{
		skip_white_space(line, &i);
		if (!line[i])
			break ;
		if (is_operator(line[i]))
		{
			if (!handle_operator_token(line, &i, &tokens))
				return (NULL);
		}
		else
		{
			if (!handle_word_token(line, &i, &tokens, env))
				return (NULL);
		}
	}
	return (tokens);
}
