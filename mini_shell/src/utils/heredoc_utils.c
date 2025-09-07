/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 15:15:02 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/07 15:15:04 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	buf_n_equal(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return (0);
		i++;
	}
	return (1);
}

int	line_buffer_grow(t_linebuf *line_buffer)
{
	size_t	new_cap;
	char	*tmp;
	size_t	i;

	new_cap = HD_INIT_CAP;
	if (line_buffer->cap != 0)
	{
		new_cap = line_buffer->cap * 2;
		if (new_cap < line_buffer->cap)
			return (-1);
	}
	tmp = malloc(new_cap);
	if (!tmp)
		return (-1);
	i = 0;
	while (i < line_buffer->len)
	{
		tmp[i] = line_buffer->buf[i];
		i++;
	}
	free(line_buffer->buf);
	line_buffer->buf = tmp;
	line_buffer->cap = new_cap;
	return (0);
}

int	line_buffer_push_char(t_linebuf *line_buffer, char c)
{
	if (line_buffer->len + 1 > line_buffer->cap)
	{
		if (line_buffer_grow(line_buffer) == -1)
			return (-1);
	}
	line_buffer->buf[line_buffer->len] = c;
	line_buffer->len += 1;
	return (0);
}

int	line_is_delim(const char *line, size_t line_len, const char *delim)
{
	size_t	delim_len;

	delim_len = ft_strlen(delim);
	if (line_len != delim_len)
		return (0);
	return (buf_n_equal(line, delim, delim_len));
}

int	feed_line_buffer(const char *buf, ssize_t n, t_feed_ctx *ctx)
{
	ssize_t	i;

	i = 0;
	while (i < n)
	{
		if (buf[i] == '\n' || buf[i] == '\r')
		{
			if (buf[i] == '\r' && (i + 1) < n && buf[i + 1] == '\n')
				i++;
			if (line_is_delim(ctx->line_buffer->buf,
					ctx->line_buffer->len, ctx->delim))
				return (1);
			write_line_and_reset(ctx->line_buffer, ctx->write_fd);
			if (buf[i] == '\r')
				(void)!write(STDOUT_FILENO, "\n", 1);
			write_prompt();
		}
		else
		{
			if (line_buffer_push_char(ctx->line_buffer, buf[i]) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}
