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

# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <stddef.h>
# include <fcntl.h>
# include <limits.h>

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
}	t_subtype;

typedef enum e_redirect_subtype
{
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
}			t_token;

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
    int             type;
    int             subtype;
    char            **args;
    int             n_args;
    char            *red_args[2];
    int             append;
    struct s_ast    *next;
    struct s_ast    *left;
    struct s_ast    *right;
}					t_ast;

typedef struct  s_state_parser
{
	char    *str;
	int     needle;
	int     i_word;
	int     n_cmd_ltrs;
	int     error;
}			t_s_parser;

typedef struct  s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}   				t_env;

typedef struct  s_shell
{
	t_ast   *ast_head;
	t_ast   *ast_tree;
	t_env   *env;
	t_env   *var;
}			t_shell;

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
int		execute_ast(t_shell *shell, t_ast *node);
int		execute_pipe(t_shell *shell, t_ast *node);
int		ft_echo(t_ast *node);
int		ft_cd(t_shell *shell, t_ast *node);
int		ft_pwd(void);
int		ft_export(t_shell *shell, t_ast *node);
int		ft_unset(t_shell *shell, t_ast *node);
int		ft_env(t_shell *shell, t_ast *node);
void	ft_exit(t_shell *shell);
int		execute_and(t_shell *shell, t_ast *node);
int		execute_or(t_shell *shell, t_ast *node);
char  *get_cmd_path(char *cmd, char *env);
t_env *gen_env_node(char *key, char *value);
void  free_env_node(t_env **node);
void  free_env_node_by_key(t_env **head, char *key);
void  set_env_node(t_env **head, char *key, char *value);
t_env *get_env_node(t_env *head, char *key);
int 	get_env_length(t_env *head);
char  *gen_env_str(char *key, char *value);
char  **convert_env_to_list(t_env *head);
void  free_env(t_env **head);
void  print_env(t_shell *shell, int export);
void  cleanup(t_shell **shell);
void  free_env_list(char  **list);
void  print_err(char *cmd_name, char *err_msg);
char  *itoa(int num);
t_env   *init_env(void);

#endif
