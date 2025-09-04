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


static int	heredoc_read_loop(const char *delim, int write_fd, t_linebuf *line_buffer)
{
	char	buf[1024];
	ssize_t	bytes_read;
	int		chunk_status;

	while ((bytes_read = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
	{
		chunk_status = feed_line_buffer(buf, bytes_read, line_buffer, write_fd, delim);
		if (chunk_status == 1)
			return (0);
		if (chunk_status == -1)
			return (-1);
	}
	return (0);
}



static void	heredoc_child(const char *delim, int write_fd)
{
	t_linebuf	line_buffer;
	int			read_status;

	heredoc_init(&line_buffer);
	read_status = heredoc_read_loop(delim, write_fd, &line_buffer);
	if (read_status == -1)
		perror("minishell: heredoc: malloc");
	free(line_buffer.buf);
	close(write_fd);
	if (read_status == -1)
		exit(1);
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
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			g_exit_code = 130;
		close(pfd[0]);
		return (-1);
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
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
	{
		close(pfd[0]);
		heredoc_child(delimiter, pfd[1]);
	}
	return (heredoc_parent(pid, pfd, out_fd));
}
