/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:48:17 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:48:19 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	restore_signals_for_child(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

int	count_cmd(t_command *head)
{
	int	i;

	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return (i);
}

int	**alloc_pipes(int number_of_cmd)
{
	int	i;
	int	**pipes;

	i = 0;
	if (number_of_cmd < 2)
		return (NULL);
	pipes = (int **)malloc(sizeof(int *) * (number_of_cmd - 1));
	if (!pipes)
		return (NULL);
	while (i < number_of_cmd - 1)
	{
		pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return (NULL);
		}
		pipes[i][0] = -1;
		pipes[i][1] = -1;
		i++;
	}
	return (pipes);
}

void	free_pipes(int **pipes, int number_of_cmd)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < number_of_cmd - 1)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

void	close_all_pipes(int **pipes, int number_of_cmd)
{
	int	i;

	if (!pipes)
		return ;
	i = 0;
	while (i < number_of_cmd - 1)
	{
		if (pipes[i][0] != -1)
			close(pipes[i][0]);
		if (pipes[i][1] != -1)
			close(pipes[i][1]);
		pipes[i][0] = -1;
		pipes[i][1] = -1;
		i++;
	}
}
