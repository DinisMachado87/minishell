/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:37:44 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/01 19:28:07 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_nd_list(t_ast *ast, char *testname)
{
	if (!ast)
	{
		printf("== %s == \n NO AST \n", testname);
		return ;
	}
	while (ast)
	{
		print_ast_nd(ast, testname, 0);
		ast = ast->next;
	}
}

void	print_ast_core(t_ast *ast, int *indent)
{
	if (ast->left || ast->right)
		(*indent)++;
	if (ast->left)
		print_ast_core(ast->left, indent);
	if (ast->left || ast->right)
		(*indent)--;
	print_ast_nd(ast, 0, *indent);
	if (ast->left || ast->right)
		(*indent)++;
	if (ast->right)
		print_ast_core(ast->right, indent);
	if (ast->left || ast->right)
		(*indent)--;
}

void	print_ast(t_ast *ast, char *testname)
{
	int	indent;

	indent = 0;
	printf("== %s ==\n", testname);
	if (!ast)
	{
		printf(" NO AST \n");
		return ;
	}
	print_ast_core(ast, &indent);
}
