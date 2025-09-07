/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:28:13 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:28:16 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static const t_builtin	*get_builtins(void)
{
	static const t_builtin	tab[] = {
	{"echo", ft_echo},
	{"pwd", ft_pwd},
	{"env", ft_env},
	{"cd", ft_cd},
	{"export", ft_export},
	{"exit", ft_exit},
	{"unset", ft_unset},
	{NULL, NULL}
	};

	return (tab);
}

int	cmd_is_builtin(const char *cmd)
{
	int				i;
	const t_builtin	*tab;

	if (!cmd)
		return (0);
	tab = get_builtins();
	i = 0;
	while (tab[i].name)
	{
		if (ft_strncmp(cmd, tab[i].name,
				ft_strlen(tab[i].name) + 1) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	exec_builtin(char **argv, t_env **env)
{
	int				i;
	const t_builtin	*tab;

	if (!argv || !argv[0])
		return (0);
	tab = get_builtins();
	i = 0;
	while (tab[i].name)
	{
		if (ft_strncmp(argv[0], tab[i].name,
				ft_strlen(tab[i].name) + 1) == 0)
			return (tab[i].function(argv, env));
		i++;
	}
	return (-1);
}
