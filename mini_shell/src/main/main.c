/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:44:25 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:44:26 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

int	main(int argc, char **argv, char **envp)
{
	extern int	rl_catch_signals;

	(void)argc;
	(void)argv;
	rl_catch_signals = 0;
	prompt_loop(envp);
	return (g_exit_code);
}
