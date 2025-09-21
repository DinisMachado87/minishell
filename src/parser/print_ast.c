/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:37:44 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/20 14:24:27 by dimachad         ###   ########.fr       */
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

// static void print_arg_arr(char *name, char *arg, int *exp, char **tk_arr)
// {
// 	printf("%s:", name);
// 	printf("%s\t|", arg);
// 	if (tk_arr)
// 		while (*tk_arr)
// 			printf("%d:%s:", *(exp++), *(tk_arr++));
// 	else
// 		printf("NONE");
// };
//
void print_args_arr(char *print_str, t_args *args, int indent)
{
	int	i;

	i = 0;
	print_indent(indent);
	printf("|%s:%d\t", print_str, args->n);
	while (i < args->n)
	{
		printf("[%d:", args->type[i]);
		printf("%d:", args->exp[i]);
		printf("%d", args->space[i]);
		print_ast_str("", args->tkns[i], 0, "]\t");
		i++;
	}
	printf("\n");
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
	if (!ast)
		return;
	if (testname)
		printf("== %s ==\n\n", testname);
	print_indent(indent);

	printf("|%d ", ast->type);
	printf("TYPE:%s\t", type_str[ast->type]);

	printf("%d ", ast->subtype);
	print_ast_str("SUBTYPE", (char *)subtype_str[ast->subtype], 0, "\n");

	printf("[tp:ex:sp:arg:]\n");
	print_args_arr("N_ARGS", &ast->args[0], indent);
	print_args_arr("RED_IN", &ast->args[IN], indent);
	print_args_arr("RED_OUT", &ast->args[OUT], indent);
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
	printf("== %s ==\n", testname);
	if (!ast)
	{
		printf(" NO AST \n");
		return;
	}
	print_ast_core(ast, &indent);
}
