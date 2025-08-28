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
	ast->space_args = malloc((n_strs + 1) * sizeof(int));
	if (!ast->space_args)
		return (perror("Err allocating args array"), 0);
	ms_bzero(ast->args, (n_strs + 1) * sizeof(char *));
	ms_bzero(ast->exp_args, (n_strs + 1) * sizeof(int));
	ms_bzero(ast->space_args, (n_strs + 1) * sizeof(int));
	ast->n_args = n_strs;
	return (1);
}

int	allocate_red_args(t_ast *ast, int n_strs, int subtype)
{
	ast->pre_red_args[subtype] = malloc((n_strs + 1) * sizeof(char *));
	if (!ast->pre_red_args[subtype])
		return (perror("Err allocating redirect args array"), 0);
	ast->red_exp_args[subtype] = malloc((n_strs + 1) * sizeof(int));
	if (!ast->red_exp_args[subtype])
		return (perror("Err allocating redirect args array"), 0);
	ms_bzero(ast->pre_red_args[subtype], (n_strs + 1) * sizeof(char *));
	ms_bzero(ast->red_exp_args[subtype], (n_strs + 1) * sizeof(int));
	return (1);
}
