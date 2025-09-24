/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebouali <ebouali@student.s19.be>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:59:41 by ebouali           #+#    #+#             */
/*   Updated: 2025/09/22 14:59:44 by ebouali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/parse.h"

static void	exec_cmds(t_command *cmds, t_env *env)
{
	int	n;

	if (!cmds)
		return ;
	n = count_cmd(cmds);
	if (n > 1)
		execute_pipeline(cmds, env);
	else
		handle_command(cmds, env);
}

static void	process_line(char *line, t_env *env)
{
	t_token		*toks;
	t_command	*cmds;

	if (is_blank_line(line))
		return ;
	toks = convert_line_to_tokens(line, env);
	if (!toks)
	{
		ft_putendl_fd("Lexing failed: invalid input", 2);
		return ;
	}
	if (!is_valid_line(line, toks))
	{
		free_token(toks);
		return ;
	}
	cmds = parsing_cmds(&toks);
	if (!cmds || !normalize_parsed_cmds(cmds))
	{
		free_token(toks);
		return ;
	}
	exec_cmds(cmds, env);
	free_token(toks);
	free_cmd_list(cmds);
}

static int	handle_eof(char *line)
{
	if (!line)
	{
		ft_putendl_fd("exit", 1);
		g_exit_code = 0;
		return (1);
	}
	return (0);
}

void	prompt_loop(char **envp)
{
	char	*line;
	t_env	*env;

	env = init_env(envp);
	setup_prompt_signals();
	while (1)
	{
		line = readline("minishell$ ");
		if (handle_eof(line))
			break ;
		if (is_blank_line(line))
		{
			free(line);
			continue ;
		}
		add_history(line);
		process_line(line, env);
		free(line);
	}
	free_env(&env);
}
