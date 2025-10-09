/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:27:18 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/09 22:21:46 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	free_and_null_args_in_heredoc(t_args *args)
{
	int	i;

	i = 0;
	while (i < args->n)
		free_and_null((void **)&args->tkns[i++]);
	free_and_null((void **)&args->tkns);
	free_and_null((void **)&args->exp);
	free_and_null((void **)&args->space);
	free_and_null((void **)&args->type);
	return (1);
}

t_ast	*free_ast_in_heredoc_fork(t_shell *sh)
{
	t_ast	*next_head;

	while (sh->list)
	{
		next_head = sh->list->next;
		if (sh->list->type == CMD)
		{
			free_and_null_args_in_heredoc(&sh->list->args[0]);
			free_and_null_args_in_heredoc(&sh->list->args[OUT]);
			free_and_null_args_in_heredoc(&sh->list->args[IN]);
		}
		free_and_null((void **)&sh->list);
		sh->list = next_head;
	}
	return (NULL);
}
