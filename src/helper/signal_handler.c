/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 09:10:12 by jlind             #+#    #+#             */
/*   Updated: 2025/09/30 09:10:26 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
