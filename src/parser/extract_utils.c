/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:36:48 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/06 14:46:28 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <strings.h>

int	allocate_ast_args(t_ast *ast, int n_strs)
{
	ast->args = malloc((n_strs + 1) * sizeof(char *));
	if (!ast->args)
		return (perror("Err allocating args array"), 0);
	ast->exp_args = malloc((n_strs + 1) * sizeof(int));
	if (!ast->exp_args)
		return (perror("Err allocating args array"), 0);
	bzero(ast->args, n_strs);
	bzero(ast->exp_args, n_strs);
	ast->n_args = n_strs;
	return (1);
}
