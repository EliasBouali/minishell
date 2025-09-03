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

typedef int				(*t_builtin_fn)(char **argv, t_env **env);

typedef struct s_builtin
{
	const char			*name;
	t_builtin_fn		function;
}						t_builtin;

static const t_builtin	make_builtin[] = {{"echo", ft_echo}, {"pwd", ft_pwd},
		{"env", ft_env}, {"cd", ft_cd}, {"export", ft_export}, {"exit",
		ft_exit}, {"unset", ft_unset}, {NULL, NULL}};

// verifie si cmd correspond exactement a un builtin de make builtin
int	cmd_is_builtin(const char *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return (0);
	while (make_builtin[i].name)
	{
		if (ft_strncmp(cmd, make_builtin[i].name,
				ft_strlen(make_builtin[i].name) + 1) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	exec_builtin(char **argv, t_env **env)
{
	int	i;

	i = 0;
	if (!argv || !argv[0])
		return (0);
	while (make_builtin[i].name)
	{
		if (ft_strncmp(argv[0], make_builtin[i].name,
				ft_strlen(make_builtin[i].name) + 1) == 0)
		{
			return (make_builtin[i].function(argv, env));
		}
		i++;
	}
	return (-1);
	// un retour different de zero c est le comportement attentu quand une erreure arrive et que ce n est pas un builtin
}
