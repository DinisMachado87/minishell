/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:31:32 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/06 15:36:44 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ERROR -1

# include "../unity/src/unity.h"
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <stddef.h>

typedef enum e_type {
	CMD,
	REDIRECT,
	PIPE,
	LOGICAL,
} t_type;

typedef enum e_subtype {
	NONE,
	EXTERNAL,
	VAR,
	SUBSHELL,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	OUT,
	IN,
	APPEND,
	HEREDOC,
	OR,
	AND,
} t_subtype;

typedef struct s_ast
{
	int				type;
	char			**args;
	int				n_args;
	struct s_ast	*next;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

typedef struct	s_state_parser
{
	char	*str;
	int		needle;
	int 	i_word;
	int		n_cmd_ltrs;
	int		error;
}				t_s_parser;

void	prompt_loop(void);
// ast_utils
t_ast	*make_node(t_ast **ast);
t_ast	*free_all(t_ast *ast);
// parser_utils
int		is_operator(char *str);
char	*ms_strcpy(char *str, int len);
// extract_utils
int	allocate_ast_args(t_ast *ast, int n_strs);
// gen_utils
void	ms_bzero(void *s, size_t n);
// parser
t_ast	*parser(char *str);
// extract cmd
t_ast	*extract_cmd(t_ast **ast_nd, char **str, t_s_parser *s);
t_ast	*extract_subshell(t_ast **ast_nd, char **str);
t_ast	*extract_operator(t_ast **ast_nd, char **str, int operator);
// structure_ast
t_ast	*structure_ast(t_ast *cur_list);
// print_ast
void	print_ast(t_ast *ast, char *testname);
void	print_nd_list(t_ast *ast, char *testname);

#endif
