/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 19:19:54 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/01 19:26:29 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

const char	*g_type_str[] = {
	"SUBSHELL",
	"CMD",
	"REDIRECT",
	"PIPE",
	"OPERATOR",
};

const char	*g_subg_type_str[] = {
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

const char	*g_redirect_g_subg_type_str[] = {
	"IN",
	"OUT",
	"APPEND",
	"HEREDOC",
};

static void	print_ast_str(char *name, char *element, int indent, char *new_line)
{
	int	i;

	while (indent--)
		printf("\t");
	if (element)
		printf("%s:%s:", name, element);
	else
		printf("%s:NONE", name);
	printf("%s", new_line);
}

void	print_args_arr(char *print_str, t_args *args, int indent)
{
	int	i;

	i = 0;
	while (indent--)
		printf("\t");
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
}

void	print_node_type(t_ast *node, char *name, int indent, char *new_line)
{
	while (indent--)
		printf("\t");
	if (node)
		printf("%s:%s\t", name, g_type_str[node->type]);
	else
		printf("%s:NONE\t", name);
	printf("%s", new_line);
}

static void	print_ast_nd(t_ast *ast, char *testname, int indent)
{
	if (!ast)
		return ;
	if (testname)
		printf("== %s ==\n\n", testname);
	while (indent--)
		printf("\t");
	printf("|%d ", ast->type);
	printf("TYPE:%s\t", g_type_str[ast->type]);
	printf("%d ", ast->subtype);
	print_ast_str("SUBTYPE", (char *)g_subg_type_str[ast->subtype], 0, "\n");
	printf("[tp:ex:sp:arg:]\n");
	print_args_arr("N_ARGS", &ast->args[0], indent);
	print_args_arr("RED_IN", &ast->args[IN], indent);
	print_args_arr("RED_OUT", &ast->args[OUT], indent);
	print_node_type(ast->next, "|NEXT", indent, "");
	print_node_type(ast->left, "LEFT", 0, "");
	print_node_type(ast->right, "RIGHT", 0, "\n");
	while (indent--)
		printf("\t");
	printf("=====\n\n");
}
