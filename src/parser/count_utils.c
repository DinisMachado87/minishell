/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 19:59:49 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/02 20:53:16 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_end_quote(char *str, t_token *cur, t_token *nxt, int *i_ltr)
{
	if (str[*i_ltr] == cur->limiter)
	{
		if (cur->limiter != ' ')
			(*i_ltr)++;
		nxt->limiter = ' ';
	}
	else
		nxt->limiter = cur->limiter;
	if (str[*i_ltr] && is_space(str[*i_ltr]) && (cur->limiter == ' '
			|| nxt->limiter == ' '))
		cur->space_after = SPACE_AFTER;
}

int	open_fds(int *old_fd, int *new_fd, char *filename, char *exp_file)
{
	*old_fd = open(filename, O_RDONLY);
	if (*old_fd < 0)
		return (perror("Error: cannot open heredoc file"), 0);
	*new_fd = open(exp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*new_fd < 0)
		return (close(*old_fd),
			perror("Error: cannot open new heredoc file"), 0);
	return (1);
}
