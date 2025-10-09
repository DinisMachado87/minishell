/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:37:35 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/10 00:13:01 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_prompt(t_shell *shell)
{
	char	*prompt;
	char	*user;
	char	*pwd;

	user = get_env_node(shell->env, "USER")->value;
	pwd = get_env_node(shell->env, "PWD")->value;
	prompt = malloc((ms_strlen(pwd) + ms_strlen(user) + 6));
	if (!prompt)
		return (NULL);
	prompt[0] = '\0';
	ms_strncat(prompt, user, ms_strlen(user));
	ms_strncat(prompt, ":", 2);
	ms_strncat(prompt, pwd, ms_strlen(pwd));
	ms_strncat(prompt, "$ ", 3);
	return (prompt);
}

void	prompt_loop(char **input, char **prompt, t_shell *shell)
{
	int	eof;

	while (1)
	{
		eof = 0;
		*prompt = get_prompt(shell);
		get_input(*prompt, input, &eof);
		free_and_null((void **)prompt);
		if (*input)
		{
			shell->input = input;
			if (*input && **input && (parser(*input, shell, NULL) < 0))
				free_ast(shell);
			free_and_null((void **)input);
			shell->input = NULL;
			if (shell->ast)
			{
				execute_ast(shell, shell->ast);
				free_ast(shell);
			}
		}
		if (eof)
			break ;
	}
}

int	prompt_loop_wrapper(char *envp[])
{
	int		final_exit;
	char	*input;
	char	*prompt;
	t_shell	shell;

	input = NULL;
	ms_bzero((void *)&shell, sizeof(t_shell));
	init_env(&shell, envp);
	set_handler(0);
	signal(SIGQUIT, SIG_IGN);
	prompt_loop(&input, &prompt, &shell);
	final_exit = shell.exit_status;
	free_shell(&shell);
	free_and_null((void **)&prompt);
	free_and_null((void **)&input);
	return (final_exit);
}
