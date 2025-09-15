/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:37:35 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/16 13:36:41 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

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

char	*get_prompt(void)
{
	char	*cwd;
	char	*prompt;
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

int reassign_and_null(char **dst, char **src)
{
	*dst = *src;
	*src = NULL;
	return (1);
}

int	extend_str(char **dst, char **src)
{
	char	*str;
	char	*ext;
	char	*catstr;
	int		i;

	if (!dst || !src || !*dst || !*src)
		return (0);
	str = *dst;
	ext = *src;
	catstr = NULL;
	i = 0;
	if (!safe_malloc((void **)&catstr, ms_strlen(str) + ms_strlen(ext) + 1))
		return (perror("Error: Malloc input:"), 0);
	while (*str)
		catstr[i++] = *str++;
	while (*ext)
		catstr[i++] = *ext++;
	catstr[i] = '\0';
	free_and_null((void **)dst);
	free_and_null((void **)src);
	*dst = catstr;
	return (1);
}

int store_or_cat_input(char **new_read, char **input)
{
	if (new_read
		&& ((!*input && reassign_and_null(input, new_read))
			|| (*input && extend_str(input, new_read))))
		return (1);
	free_and_null((void **)input);
	free_and_null((void **)&new_read);
	return (0);
}

/*
 * On EOF readline returns NULL
 */
int	get_input(char *prompt, char **input)
{
	char	*new_read;
	int		even;

	even = 0;
	set_handler(1);
	while (!even || !*input)
	{
		if (!*input)
			new_read = readline(prompt);
		else
			new_read = readline("> ");
		if (!store_or_cat_input(&new_read, input))
			return (set_handler(0), ERROR);
		even = str_pairs_even(*input);
	}
	set_handler(0);
	if (even != ERROR && *input && **input != '\0')
		add_history(*input);
	return (even);
}

/*
 * SIGINT = ctrl_c
 * SIGQUIT = ctrl_\
 */
void	prompt_loop(char *envp[])
{
	char	*input;
	char	*prompt;
	t_shell	shell;

	input = NULL;
	ms_bzero((void *)&shell, sizeof(t_shell));
	init_env(&shell, envp);
	set_handler(0);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		prompt = get_prompt();
		if (!prompt || get_input(prompt, &input) == ERROR)
			break;
		shell.ast_tree = parser(input, &shell.ast_head);
		if (DEBUG)
			print_ast(shell.ast_tree, "loop");
		if (shell.ast_tree)
		{
			execute_ast(&shell, shell.ast_tree);
			if (DEBUG)
				printf("status = %d\n", shell.exit_status);
			free_ast(&shell.ast_head);
		}
		free_and_null((void **)&prompt);
		free_and_null((void **)&input);
	}
	free_and_null((void **)&prompt);
	free_and_null((void **)&input);
}
