/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:42:47 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/03 19:42:49 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	safe_dup2(int old_fd, int new_fd)
{
	if (old_fd == new_fd)
		return (0);
	if (old_fd < 0)
	{
		perror("minishell: dup2");
		return (-1);
	}
	if (dup2(old_fd, new_fd) == -1)
	{
		perror("minishell: dup2");
		return (-1);
	}
	return (0);
}

static int	apply_one_redirection(t_redir *redirection)
{
	int	validate;

	if (redirection->type == REDIR_INPUT || redirection->type == REDIR_HEREDOC)
		validate = safe_dup2(redirection->fd, STDIN_FILENO);
	else
		validate = safe_dup2(redirection->fd, STDOUT_FILENO);
	if (validate == -1)
		return (-1);
	if (redirection->fd > STDERR_FILENO)
		close(redirection->fd);
	redirection->fd = -1;
	return (0);
}

static void	print_open_error(const char *target)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)target, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd((char *)strerror(errno), 2);
}

static int	open_one_redirection(t_redir *redirection, t_env **env)
{
	(void)env;
	if (redirection->type == REDIR_INPUT)
		redirection->fd = open(redirection->target, O_RDONLY);
	else if (redirection->type == REDIR_OUTPUT)
		redirection->fd = open(redirection->target,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redirection->type == REDIR_APPEND)
		redirection->fd = open(redirection->target,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redirection->type == REDIR_HEREDOC)
	{
		if (open_heredoc(redirection->target, &redirection->fd, env) == -1)
			return (-1);
		return (0);
	}
	if (redirection->fd == -1)
	{
		print_open_error(redirection->target);
		if (errno == EACCES || errno == EISDIR)
			g_exit_code = 126;
		else
			g_exit_code = 1;
		return (-1);
	}
	return (0);
}

int	apply_redirections(t_command *cmd, t_env **env)
{
	t_redir	*redirection;

	if (!cmd)
		return (0);
	redirection = cmd->redir;
	while (redirection)
	{
		if (redirection->fd == -1)
		{
			if (open_one_redirection(redirection, env) == -1)
				return (-1);
		}
		if (apply_one_redirection(redirection) == -1)
			return (-1);
		redirection = redirection->next;
	}
	return (0);
}
