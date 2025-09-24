/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 16:38:38 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/04 13:47:24 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

/*fonction pour trouver le mot en double quote""*/
char	*extract_d_quoted(char *line, int *i)
{
	int		start;
	int		end;

	start = 0;
	end = 0;
	if (!line || !line[*i] || line[*i] != '"')/*verifier le debut du double quote*/
		return (NULL);
	start = *i + 1;
	end = start;
	while (line[end] && line[end] != '"')
		end++;
	if (line[end] != '"')/*verifier la fin du double quote*/
	{
		(*i) = end;
		return (NULL);
	}
	(*i) = end + 1;
	return(ft_strndup(&line[start], (size_t)(end - start)));
}
/*fonction pour trouver le mot en single quote''*/
char	*extract_s_quoted(char *line, int *i)
{
	int		start;
	int		end;

	start = 0;
	end = 0;
	if (!line || !line[*i] || line[*i] != '\'')/*verifier le debut du single quote*/
		return (NULL);
	start = *i + 1;
	end = start;
	while (line[end] && line[end] != '\'')
		end++;
	if (line[end] != '\'')/*verifier la fin du double quote*/
	{
		(*i) = end;
		return (NULL);
	}
	(*i) = end + 1;
	return(ft_strndup(&line[start], (size_t)(end - start)));
}
/*fonction pour trouver le mot sans quote*/
char	*extract_unquoted(char *line, int *i)
{
	int		start;
	int		end;

	start = 0;
	end = 0;
	if (!line || !line[*i] || is_quoted(line[*i]) ||
		is_operator(line[*i]) || is_white_space(line[*i]))
		return (NULL);
	start = *i;
	end = start;
	while (line[end]
		&& !is_quoted(line[end])
		&& !is_operator(line[end])
		&& !is_white_space(line[end]))
		end++;
	if (start == end)
		return (NULL);
	(*i) = end;
	return(ft_strndup(&line[start], (size_t)(end - start)));
}
