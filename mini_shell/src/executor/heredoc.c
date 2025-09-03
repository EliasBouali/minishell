/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:42:34 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:42:36 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	heredoc_child(const char *delim, int write_fd)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delim, ft_strlen(delim) + 1) == 0)
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	exit(0);
}

static int	heredoc_parent(pid_t pid, int pfd[2], int *out_fd)
{
	int	status;

	close(pfd[1]);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("minishell: waitpid");
		close(pfd[0]);
		return (-1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g_exit_code = 130;
		close(pfd[0]);
		return (-1);
	}
	*out_fd = pfd[0];
	return (0);
}

int	open_heredoc(const char *delimiter, int *out_fd, t_env **env)
{
	int		pfd[2];
	pid_t	pid;

	(void)env;
	if (pipe(pfd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close(pfd[0]);
		close(pfd[1]);
		return (-1);
	}
	if (pid == 0)
		heredoc_child(delimiter, pfd[1]);
	return (heredoc_parent(pid, pfd, out_fd));
}
