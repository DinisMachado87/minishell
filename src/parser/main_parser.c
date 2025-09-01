/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 15:33:50 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/02 00:31:31 by dimachad         ###   ########.fr       */
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
	t_ast		*ast_list;
	t_parser	s;
	int			i;
	char		*str_cmd = "echo <file.txt 'the city<file.txt\"is wilder\" than you think'\"'and think' better\"    ";
	char		*str_par = "(echo \"I am them\")";
	char		*str_pipe = "|";

	ast = NULL;
	ast_list = NULL;
	ms_bzero((void *)&s, sizeof(t_parser));
	i = 0;
	extract_cmd(&str_cmd, &s);
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
		ast = parser((char *)test_strs[i], &ast_list);
		print_ast(ast, "Parser AST");
		if (ast_list)
			ast_list = free_ast(&ast_list);
		ast = NULL;
		i++;
	}
	if (argc == 2)
	{

		ast_list = parser(argv[1], &ast);
		printf("== %s ==", argv[1]);
		print_nd_list(ast, "Parser List");
		print_ast(ast_list, "Parser AST");
		ast = free_ast(&ast);
		ast_list = NULL;
	}
	return (0);
}
