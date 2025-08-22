/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:33:50 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/22 02:06:48 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

const char *test_strs[] = {
    // "   echo Hello | cat $signal.txt   ",
    // "",
    // "   echo Hello <text.txt | cat signal.txt> text.txt   ",
    // "   echo Hello | (cat signal.txt <text.txt ) ",
    // "   (cat signal.txt <text.txt ) | echo Hello && ls -l   ",
    // "   echo Hello | cat signal.txt <text.txt  ",
    // "   cat signal.txt <text.txt  | echo Hello && ls -l   ",
    // "   ls -l || echo Hello | cat $signal.txt   ",
    // "echo 'the city \"is wilder\" than you think' (ls) \"(ls)\" \"'and think' better\"   ",
    //"   <<eof cat signal.txt> text.txt   ",
	"< $env cat",
	"cat '$env'$cat\"$double\"'$env'",
	NULL,
};

int	main(int argc, char **argv)
{
	t_ast		*ast;
	t_ast		*ast_ast;
	t_s_parser	s;
	int			i;
	char		*str_cmd = "echo <file.txt 'the city<file.txt\"is wilder\" than you think'\"'and think' better\"    ";
	char		*str_par = "(echo \"I am them\")";
	char		*str_pipe = "|";

	ast = NULL;
	i = 0;
	extract_cmd(&ast, &str_cmd, &s);
	print_nd_list(ast, "Extract Cmd");
	ast = free_ast(&ast);
	extract_subshell(&ast, &str_par);
	print_nd_list(ast, "Extract Subshell");
	ast = free_ast(&ast);
	extract_operator(&ast, &str_pipe, PIPE);
	print_nd_list(ast, "Extract operator");
	ast = free_ast(&ast);
	while (test_strs[i])
	{
		printf("== %s ==\n", test_strs[i]);
		ast_ast = parser((char *)test_strs[i], &ast);
		print_ast(ast_ast, "Parser AST");
		ast = free_ast(&ast);
		ast_ast = NULL;
		i++;
	}
	if (argc == 2)
	{

		ast_ast = parser(argv[1], &ast);
		printf("== %s ==", argv[1]);
		print_nd_list(ast, "Parser List");
		print_ast(ast_ast, "Parser AST");
		ast = free_ast(&ast);
		ast_ast = NULL;
	}
	return (0);
}
