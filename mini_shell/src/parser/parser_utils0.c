/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kingstephane <kingstephane@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 15:05:02 by kingstephan       #+#    #+#             */
/*   Updated: 2025/09/02 19:05:51 by kingstephan      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

/*fonction pour compter les nombre de tokens
de type word qui seront ajouter dans les args
de ma commande*/
int	nb_args(t_token **token)
{
	int		count;
	t_token	*tmp;

	count = 0;
	tmp = *token;
	while (tmp && tmp->type != TOKEN_PIPE)
	{
		if (tmp->type == TOKEN_WORD)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

/*fonction pour allouer et initialiser ma commande
avec la bonne taille pour les arguments et mettre
tous les points de la struct commande a NULL ou
negatif.*/
t_command	*init_cmd(int word_count)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->argv = malloc(sizeof(char *) * (word_count + 1));
	if (!new_cmd->argv)
	{
		free(new_cmd);
		return (NULL);
	}
	new_cmd->append = -1;
	new_cmd->heredoc = -1;
	new_cmd->next = NULL;
	new_cmd->infile = NULL;
	new_cmd->outfile = NULL;
	return (new_cmd);
}

/*ma fonction ft_strdup qui libere la liste
de commande en cas d'erreur de malloc.*/
char	*safe_ft_strdup(char *s, t_command **cmd_lst)
{
	char	*dup;

	if (!s)
		return (NULL);
	dup = ft_strdup(s);
	if (!dup)
	{
		if (cmd_lst && *cmd_lst)
		{
			free_cmd_list(*cmd_lst);
			*cmd_lst = NULL;
		}
		return (NULL);
	}
	return (dup);
}

/*fonction qui liberer mes args dans ma commande
en cas d'erreur en plein remplissage*/
void	clean_cmd_argv(char **argv, int count)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (i < count)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

/*fonction qui verifie si mon token dans
ma ligne de commande est une redirection.*/
int	is_redirection(t_token *token)
{
	if (token->type == TOKEN_REDIR_IN)
		return (1);
	else if (token->type == TOKEN_HEREDOC)
		return (1);
	else if (token->type == TOKEN_REDIR_OUT)
		return (1);
	else if (token->type == TOKEN_REDIR_APPEND)
		return (1);
	else
		return (0);
}
