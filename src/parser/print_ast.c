/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:37:44 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/30 17:42:48 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

const char *type_str[] = {
	"SUBSHELL",
	"CMD",
	"REDIRECT",
	"PIPE",
	"OPERATOR",
};

const char *subtype_str[] =
{
	"NONE",
	"EXTERNAL",
	"VAR",
	"ECHO",
	"CD",
	"PWD",
	"EXPORT",
	"UNSET",
	"ENV",
	"EXIT",
	"OR",
	"AND",
};

const char *redirect_subtype_str[] = {
	"IN",
	"OUT",
	"APPEND",
	"HEREDOC",
};

static void	print_indent(int indent)
{
	while (indent)
	{
		printf("\t");
		indent--;
	}
}

static void print_ast_str(char *name, char *element, int indent, char *new_line)
{
	print_indent(indent);
	if (element)
		printf("%s:%s:", name, element);
	else
		printf("%s:NONE", name);
	printf("%s", new_line);
};

static void print_arg_arr(char *name, char *arg, int *exp, char **tk_arr)
{
	printf("%s:", name);
	printf("%s\t|", arg);
	if (tk_arr)
		while (*tk_arr)
			printf("%d:%s:", *(exp++), *(tk_arr++));
	else
		printf("NONE");
};

static void print_ast_arr(char **arr, int *exarr, int *spacearr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		printf("%d:", exarr[i]);
		printf("%d", spacearr[i]);
		print_ast_str("", arr[i], 0, "\t");
		i++;
	}
	printf("[ex:sp:arg:]");
};

static void	print_node_type(t_ast *node, char *name, int indent, char *new_line)
{
	print_indent(indent);
	if (node)
		printf("%s:%s\t", name, type_str[node->type]);
	else
		printf("%s:NONE\t", name);
	printf("%s", new_line);
}

static void	print_ast_nd(t_ast *ast, char *testname, int indent)
{
	if (testname)
		printf("== %s ==\n\n", testname);
	print_indent(indent);
	printf("|%d ", ast->type);
	printf("TYPE:%s\t", type_str[ast->type]);
	if (ast->type == REDIRECT)
	{
		printf("%d ", ast->subtype);
		print_ast_str("SUBTYPE", (char *)redirect_subtype_str[ast->subtype], 0, "\n");
	}
	else
	{
		printf("%d ", ast->subtype);
		print_ast_str("SUBTYPE", (char *)subtype_str[ast->subtype], 0, "\n");
	}
	print_indent(indent);
	printf("|N_ARGS:%d\t", ast->n_args);
	print_ast_arr(ast->args, ast->exp_args, ast->space_args);
	printf("\n");
	print_indent(indent);
	print_arg_arr("|RED_IN", ast->red_args[IN], ast->r_exp_args[IN], ast->pre_r_args[IN]);
	print_arg_arr("\tRED_OUT", ast->red_args[OUT], ast->r_exp_args[OUT], ast->pre_r_args[OUT]);
	printf("\n");
	print_node_type(ast->next, "|NEXT", indent, "");
	print_node_type(ast->left, "LEFT", 0, "");
	print_node_type(ast->right, "RIGHT", 0, "\n");
	print_indent(indent);
	printf("=====\n\n");
}


void	print_nd_list(t_ast *ast, char *testname)
{
	if (!ast)
	{
		printf("== %s == \n NO AST \n", testname);
		return;
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
	printf("== %s ==\n\n", testname);
	if (!ast)
	{
		printf(" NO AST \n");
		return;
	}
	print_ast_core(ast, &indent);
}
