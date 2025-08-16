/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:37:35 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/14 20:28:37 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	sig_c_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
}

void	sig_c_rdline_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

void	set_handler(int rdline)
{
	struct sigaction	sa_c;

	sa_c.sa_flags = 0;
	if (rdline)
		sa_c.sa_flags = SA_RESTART;
	sigemptyset(&sa_c.sa_mask);
	sa_c.sa_handler = sig_c_handler;
	if (rdline)
		sa_c.sa_handler = sig_c_rdline_handler;
	if (sigaction(SIGINT, &sa_c, NULL) == -1)
		perror("sigaction");
}

char  *get_prompt(void)
{
	char  *cwd;
	char  *prompt;
	char	*user;

	cwd = getcwd(NULL, 0);
	user = getenv("USER");
	if (!cwd)
	  return (NULL);
	prompt = malloc((ms_strlen(cwd) + ms_strlen(user) + 6));
	if (!prompt)
	{
	  free(cwd);
	  return (NULL);
	}
	prompt[0] = '\0';
	ms_strncat(prompt, user, ms_strlen(user));
	ms_strncat(prompt, ":", 2);
	ms_strncat(prompt, cwd, ms_strlen(cwd));
	ms_strncat(prompt, "$ ", 3);
	free(cwd);
	return (prompt);
}

/*
 * On EOF readline returns NULL
 */
char  *get_input(char *prompt)
{
	char  *input;

	set_handler(1);
	input = readline(prompt);
	set_handler(0);
	if (!input)
	{
		free(prompt);
		exit(1);
	}
	if (*input)
		add_history(input);
	return (input);
}

/*
 * SIGINT = ctrl_c
 * SIGQUIT = ctrl_\
 */
void	prompt_loop(void)
{
	char  *input;
	char  *prompt;
	t_shell	shell;

	ms_bzero((void *)&shell, sizeof(t_shell));
	shell.env = init_env();
	set_handler(0);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		prompt = get_prompt();
		if (!prompt)
			return ;
		input = get_input(prompt);
		if (!input)
		{
			free(prompt);
			return ;
		}
		shell.ast_tree = parser(input, &shell.ast_head);
		print_ast(shell.ast_tree, "loop");
		execute_ast(&shell, shell.ast_tree);
		printf("status = %d\n", shell.exit_status);
		free_all(&shell.ast_head);
		free(prompt);
		free(input);
		prompt = NULL;
		input = NULL;
	}
	if (input)
		free(input);
	free(prompt);
}
