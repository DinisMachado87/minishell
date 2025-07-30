/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:33:50 by dimachad          #+#    #+#             */
/*   Updated: 2025/07/30 01:39:47 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

const char *type_str[] = {
	"CMD",
	"PIPE",
	"REDIRECT_OUT",
	"REDIRECT_IN",
	"SUBSHELL",
	"OR",
	"AND",
	"APPEND",
	"HEREDOC"
};

void	print_ast_nd(t_ast *ast, char *testname)
{
	int	i;

	i = 0;
	if (testname)
		printf("== %s ==\n\n", testname);
	printf("TYPE:\t%s\n", type_str[ast->type]);
	if (ast->args)
	{
		printf("ARGS:");
		while (ast && ast->args[i])
		{
			printf("\t%s\n", ast->args[i]);
			i++;
		}
	}
	if (ast->n_args)
		printf("N_ARGS:\t%d\n", ast->n_args);
	if (ast->next)
		printf("TYPE:\t%s\n", type_str[ast->next->type]);
	if (ast->left)
		printf("TYPE:\t%s\n", type_str[ast->left->type]);
	if (ast->right)
		printf("TYPE:\t%s\n", type_str[ast->right->type]);
	printf("\n");
}


void	print_nd_list(t_ast *ast, char *testname)
{
	while (ast)
	{
		print_ast_nd(ast, testname);
		ast = ast->next;
	}
}

void	print_ast(t_ast *ast, char *testname)
{
	while (ast)
	{
		print_ast_nd(ast, testname);
		ast = ast->next;
	}
}

int	main()
{
	t_ast	*ast;
	// t_s_parser s;
	// char	*str_cmd = "echo 'the city \"is wilder\" than you think'\"'and think' better\"    ";
	// char	*str_par = "(echo \"I am them\")";
	// char	*str_pipe = "|";
	char	*str_input = "echo 'the city \"is wilder\" than you think' (ls) \"(ls)\" \"'and think' better\"   ";

	ast = NULL;
	// extract_cmd(&ast, &str_cmd, &s);
	// print_ast(ast, "Extract Cmd");
	// ast = free_all(ast);
	// extract_subshell(&ast, &str_par);
	// print_ast(ast, "Extract Subshell");
	// ast = free_all(ast);
	// extract_operator(&ast, &str_pipe, PIPE);
	// print_ast(ast, "Extract operator");
	// ast = free_all(ast);
	ast = parser(str_input);
	print_ast(ast, "Parser");
	ast = free_all(ast);
	return (0);
}
