/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:29:36 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:29:38 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_unset_invalid(const char *arg)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int	ft_unset(char **argv, t_env **env)
{
	int	i;
	int	exit_code;

	if (!env || !argv)
		return (1);
	if (!argv[1])
		return (0);
	exit_code = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_name((const char *)argv[i]))
		{
			print_unset_invalid(argv[i]);
			exit_code = 1;
		}
		else
			env_unset(env, (const char *)argv[i]);
		i++;
	}
	return (exit_code);
}
