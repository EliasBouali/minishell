#include "../include/minishell.h"

void	write_prompt(void)
{
	(void)!write(STDOUT_FILENO, "> ", 2);
}

void	write_line_and_reset(t_linebuf *line_buffer, int write_file_descriptor)
{
	if (line_buffer->len)
		(void)!write(write_file_descriptor, line_buffer->buf, line_buffer->len);
	(void)!write(write_file_descriptor, "\n", 1);
	line_buffer->len = 0;
}

void	heredoc_init(t_linebuf *line_buffer)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	line_buffer->buf = NULL;
	line_buffer->len = 0;
	line_buffer->cap = 0;
	write_prompt();
}
