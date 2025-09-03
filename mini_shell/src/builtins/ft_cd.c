/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:28:23 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:28:26 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static const char	*get_home(t_env *env)
{
	const char	*home;

	home = env_get(env, "HOME");
	if (!home || !*home)
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	return (home);
}

static int	update_pwd_vars(t_env **env, const char *oldpwd)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (perror("minishell: getcwd"), 1);
	if (env_set(env, "OLDPWD", (char *)oldpwd) == 2 || env_set(env, "PWD",
			cwd) == 2)
	{
		free(cwd);
		ft_putstr_fd("minishell: cd: alloc error\n", 2);
		return (1);
	}
	free(cwd);
	return (0);
}

int	ft_cd(char **argv, t_env **env)
{
	const char	*target;
	char		*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (perror("minishell: getcwd"), 1);
	if (argv[1])
		target = argv[1];
	else
		target = get_home(*env);
	if (!target || chdir(target) == -1)
	{
		if (target)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			perror((char *)target);
		}
		return (free(oldpwd), 1);
	}
	if (update_pwd_vars(env, oldpwd))
		return (free(oldpwd), 1);
	return (free(oldpwd), 0);
}
