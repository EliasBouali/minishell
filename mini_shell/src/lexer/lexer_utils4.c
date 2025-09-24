/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:49:28 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/02 18:50:23 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

/*fonction pour verifier le premier caractere d'une variable
si elle est valide ou pas*/
int	var_start_ok(char c)
{
	if (!ft_isalpha(c) && c != '_')
		return (0);
	return (1);
}
/*fonction pour les cas speciaux apres le signe dollar*/
int	check_special_dollar_case(char *str, int i)
{
	char	next;

	if (!str || str[i] != '$')
		return (0);
	next = str[i + 1];
	if (next == '\0')
		return (1);
	if (next == '$' || next == '"' || next == '=' || next == '\\' || ft_isdigit(next))
		return (1);
	return (0);
}
/*fonction pour sortir le nom de la variable d'environement
dans les doubles quotes*/
char	*extract_var_name(char *str, int *i)
{
	int		start;
	int		j;
	char	*var_str;

	if (!str || str[*i] != '$')
		return (NULL);
	if (str[*i + 1] == '?')
	{
		(*i) += 2;
		return (ft_strdup("?"));
	}
	if (!var_start_ok(str[*i + 1]))
		return (NULL);
	(*i)++;
	start = *i;
	j = 0;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
	{
		(*i)++;
		j++;
	}
	var_str = ft_strndup(&str[start], j);
	if (!var_str)
		return (NULL);
	return (var_str);
}
/*fonction pour sortir la valeur de la variable d'environement
ou son chemin
exemple '$USER = nom_de_hote'*/
char	*get_var_value(char *name, t_env *env)
{
	char		*var_name;
	const char	*value;

	if (!name)
		return (NULL);
	if (ft_strncmp(name, "?", 1) == 0)
		return (ft_itoa(g_exit_code));
	value = env_get(env, name);
	var_name = ft_strdup(value);
	if (!var_name)
		return (ft_strdup(""));
	return (var_name);
}
/*fonction pour trouver la taille de la value de notre variable
d'environement*/
int	get_var_len(char *str, int *i, t_env *env)
{
	int		var_len;
	char	*var_name;
	char	*var_value;

	if (!str)
		return (0);
	var_len = 0;
	var_name = extract_var_name(str, i);
	if (!var_name)
		return (0);
	var_value = get_var_value(var_name, env);
	if (var_value)
		var_len += ft_strlen(var_value);
	free(var_value);
	free(var_name);
	return (var_len);
}
