/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:31:32 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/06 16:12:38 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ERROR -1

# include "../unity/src/unity.h"
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <stddef.h>

typedef enum e_type {
	SUBSHELL,
	CMD,
	REDIRECT,
	PIPE,
	OPERATOR,
}	t_type;

typedef enum e_subtype {
	NONE,
	EXTERNAL,
	VAR,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
	OR,
	AND,
} t_subtype;

typedef enum e_redirect_subtype {
	IN,
	OUT,
	APPEND,
	HEREDOC,
}	t_redirect_subtype;

typedef struct s_token
{
	char	*str;
	int		type;
	int		subtype;
}	t_token;

static const t_token g_types[] =
{
	{"&&", OPERATOR, AND},
	{"||", OPERATOR, OR},
	{"|", PIPE, NONE},
	{"<<", REDIRECT, HEREDOC},
	{">>", REDIRECT, APPEND},
	{"<", REDIRECT, IN},
	{">", REDIRECT, OUT},
	{"VAR=", CMD, VAR},
	{"echo", CMD, ECHO},
	{"cd", CMD, CD},
	{"pwd", CMD, PWD},
	{"export", CMD, EXPORT},
	{"unset", CMD, UNSET},
	{"env", CMD, ENV},
	{"exit", CMD, EXIT},
	{0, 0, 0},
};

typedef struct s_ast
{
	int				type;
	int				subtype;
	char			**args;
	int				n_args;
	char			*red_args[2];
	int				append;
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

// ast_utils
t_ast	*make_node(t_ast **ast);
t_ast	*free_all(t_ast **ast);
// parser_utils
int		type(char *str);
int		subtype(char *str);
char	*ms_strcpy(char *str, int len);
// extract_utils
int		allocate_ast_args(t_ast *ast, int n_strs);
// gen_utils
void	ms_bzero(void *s, size_t n);
int		ms_strcmp(char *ref, char *str);
// parser
t_ast	*parser(char *str, t_ast **head_list);
// extract cmd
t_ast	*extract_cmd(t_ast **ast_nd, char **str, t_s_parser *s);
t_ast	*extract_subshell(t_ast **ast_nd, char **str);
t_ast	*extract_operator(t_ast **ast_nd, char **str, int operator);
// structure_ast
t_ast	*structure_ast(t_ast *cur_list);
// print_ast
void	print_ast(t_ast *ast, char *testname);
void	print_nd_list(t_ast *ast, char *testname);
// prompt_loop
void	prompt_loop(void);

#endif
