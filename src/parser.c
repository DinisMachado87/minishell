/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 12:22:45 by dimachad          #+#    #+#             */
/*   Updated: 2025/07/02 17:55:26 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// @Returns the ast still as a list and a pointer by reference on the current letter not split yet
// @extract extracts the first word of the input string and stores it as the first args in the s_ast node
//void	extract_word(t_ast *ast, char **str)
//{
//}

// @tokenize stores str_compares extracted word into token and stores token
//void	tokenize(t_ast *ast)
//{
//}

// @compose_node calls the right callback function to extract and store remaining arguments according to tokenized cmd
//void	compose_node(t_ast *ast, char **str)
//{
//}

// @Parser extracts action nodes into ast using the next linear list pointers
t_ast	*parser(char **str)
{
	(void)str;
	t_ast *ast;

	if (!make_node(&ast))
		free_ast();
//	extract_word(&ast, str);
//	tokenize(&ast);
//	compose_node(&ast, str);
	return (ast);
}


// @make_tree crosses ast through the list next pointers restructuring it into tree with the left and right pointers
//int	main(void)
//{
//	//t_ast	*ast_list;
//	//t_ast	*ast_tree;
//	//char	*cur_ltr;
//
//	//cur_ltr = str;
//	//ast_list = 0;
//	//while (*cur_ltr)
//	//	parser(&ast_list, &cur_ltr);
//	//make_tree(&ast_list, &ast_tree);
//	//return (0);
//	char	*str[] = {"Hello", "Bye", NULL};
//	parser(str);
//}
