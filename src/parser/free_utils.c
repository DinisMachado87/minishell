/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 20:27:18 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/18 14:36:32 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	free_and_null(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
	return (1);
}

void free_and_null_str_arr(char	***address_str_arr)
{
	int		i_str;
	char	**str_arr;

	if (!address_str_arr || !*address_str_arr)
		return ;
	i_str = 0;
	str_arr = *address_str_arr;
	while (str_arr[i_str])
		free_and_null((void **)&str_arr[i_str++]);
	free_and_null((void **)address_str_arr);
}

int	free_and_null_args(t_args *args)
{
	int i;

	i = 0;
	while (i < args->n)
	{
		if (args->type[i] == HEREDOC)
			unlink(args->tkns[i]);
		free_and_null((void **)&args->tkns[i++]);
	}
	free_and_null((void **)&args->tkns);
	free_and_null((void **)&args->exp);
	free_and_null((void **)&args->space);
	free_and_null((void **)&args->type);
	return (1);
}

t_ast	*free_ast(t_shell *sh)
{
	while ((sh->list))
	{
		t_ast *next_head;

		next_head = sh->list->next;
		free_and_null_args(&sh->ast->args[0]);
		free_and_null_args(&sh->ast->args[OUT]);
		free_and_null_args(&sh->ast->args[IN]);
		free_and_null((void **)&sh->list);
		sh->list = next_head;
	}
	return (NULL);
}

