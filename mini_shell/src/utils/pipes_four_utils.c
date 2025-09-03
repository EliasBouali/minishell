/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_four_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:47:29 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:47:30 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	setup_pipes_and_pids(int n, int ***pipes_out, pid_t **pids_out)
{
	int	**pipes;

	pipes = alloc_pipes(n);
	if ((n > 1 && !pipes) || (n > 1 && create_pipes(pipes, n) < 0))
		return (-1);
	*pids_out = (pid_t *)malloc(sizeof(pid_t) * n);
	if (!*pids_out)
	{
		if (n > 1)
		{
			close_all_pipes(pipes, n);
			free_pipes(pipes, n);
		}
		return (-1);
	}
	*pipes_out = pipes;
	return (0);
}

int	handle_spawn_error(int i, int n, pid_t *pids, int **pipes)
{
	close_all_pipes(pipes, n);
	while (i - 1 >= 0)
	{
		i--;
		waitpid(pids[i], NULL, 0);
	}
	free(pids);
	free_pipes(pipes, n);
	g_exit_code = 1;
	return (-1);
}
