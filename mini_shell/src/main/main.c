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

extern int	rl_catch_signals;

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	// Empêcher readline de gérer elle-même les signaux
	rl_catch_signals = 0;
	// Lancer la boucle du shell
	prompt_loop(env);
	return (g_exit_code);
}
