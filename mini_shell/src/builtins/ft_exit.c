/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:28:57 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:29:01 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern int	g_exit_code;

static int	is_numeric(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(char **argv, t_env **env)
{
	long long	val;

	(void)env;
	ft_putstr_fd("exit\n", 1);
	if (!argv[1])
		exit(g_exit_code);
	if (!is_numeric(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(255);
	}
	val = ft_atoll(argv[1]);
	if (argv[2])
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2), 1);
	exit((unsigned char)val);
}
