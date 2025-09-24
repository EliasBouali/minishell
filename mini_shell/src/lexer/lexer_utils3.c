/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:47:29 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/04 17:12:32 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

static int	append_S_quoted(char *line, int *i, char **final_word, t_env *env)
{
	char	*word;
	(void)	env;

	word = extract_s_quoted(line, i);
	if (!word)
		{
			free(*final_word);
			*final_word = NULL;
			return (0);
		}
	*final_word = ft_strjoin_and_free(*final_word, word);
	free(word);
	if (!*final_word)
		return (0);
	return (1);
}
static int	append_D_quoted(char *line, int *i, char **final_word, t_env *env)
{
	char	*word;
	char	*temp;

	temp = extract_d_quoted(line, i);
	if (!temp)
	{
		free(*final_word);
		*final_word = NULL;
		return (0);
	}
	word = expand_variables(temp, env);
	free(temp);
	if (!word)
	{
		free(*final_word);
		*final_word = NULL;
		return (0);
	}
	*final_word = ft_strjoin_and_free(*final_word, word);
	free(word);
	if (!*final_word)
		return (0);
	return (1);
}
static int	append_unquoted(char *line, int *i, char **final_word, t_env *env)
{
	char	*word;
	char	*temp;

	temp = extract_unquoted(line, i);
	if (!temp)
	{
		free(*final_word);
		*final_word = NULL;
		return (0);
	}
	word = expand_variables(temp, env);
	free(temp);
	if (!word)
	{
		free(*final_word);
		*final_word = NULL;
		return (0);
	}
	*final_word = ft_strjoin_and_free(*final_word, word);
	free(word);
	if (!*final_word)
		return (0);
	return (1);
}

static int	process_quote_type(char *line, int *i, char **final_word, t_env *env)
{
	if (line[*i] == '\'')
		return (append_S_quoted(line, i, final_word, env));
	else if (line[*i] == '"')
		return (append_D_quoted(line, i, final_word, env));
	else
		return (append_unquoted(line, i, final_word, env));
}

char	*collect_word(char *line, int *i, t_env *env)
{
	char	*final_word;

	final_word = NULL;
	if (!line || !line[*i] || is_white_space(line[*i])
		|| is_operator(line[*i]))
		return (NULL);
	while (line[*i])
	{
		if (!line[*i] || is_white_space(line[*i]) || is_operator(line[*i]))
			break;
		if (!process_quote_type(line, i, &final_word, env))
			return (NULL);
	}
	if (!final_word)
		return (ft_strdup(""));
	return (final_word);
}
