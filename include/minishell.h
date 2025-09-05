/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:31:32 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/05 16:21:44 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ERROR -1
# define DEBUG 0
# define TEMP_PREFIX "temp_heredoc"

# include <stdlib.h>
# include <stdio.h>
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

enum e_expand {
	DONT_EXPAND,
	EXPAND,
	POTENCIALLY_EXPAND,
};

enum e_space {
	NO_SPACE_AFTER,
	SPACE_AFTER,
};

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
}			t_s_token;

static const t_s_token g_types[] =
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
    int				*exp_args;
	int				*space_args;
    int             n_args;
    int             append;
    int             heredoc;
    char            *red_args[2];
	char			**pre_r_args[2];
    int				*r_exp_args[2];
    struct s_ast    *next;
    struct s_ast    *left;
    struct s_ast    *right;
}					t_ast;

typedef struct  s_state_parser
{
	t_ast	*ast;
	int		n_heredoc;
}			t_parser;

typedef struct s_state_token
{
	char	limiter;
	int		space_after;
	char	*str;
}	t_token;

typedef struct s_state_cmd {
	int	n_cmd_tk;
	int	i_tkn;
	int	n_red_tk[2];
	int	ltr;
	int	type;
	int	subtype;
}	t_cmd;

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
	int		exit_status;
}			t_shell;

//int		count_redirect(t_token cur, t_cmd *c);

// ast
// execute_ast.c
void	execute_ast(t_shell *shell, t_ast *node);
// cmd_expander.c
int		cmd_expander(t_ast *ast, t_env *env_head);

// commands
// export_utils.c
void	print_env(t_shell *shell, int _export);
// get_cmd_path.c
char	*get_cmd_path(char *cmd, char *env);
// built_ins.c
int		ft_echo(t_ast *node);
int		ft_cd(t_shell *shell, t_ast *node);
int		ft_pwd(void);
int		ft_export(t_shell *shell, t_ast *node);
int		ft_unset(t_shell *shell, t_ast *node);
int		ft_env(t_shell *shell, t_ast *node);
void	ft_exit(t_shell *shell);

// helper
// print_err.c
void	print_err(char *cmd_name, char *err_msg);
// itoa.c
char	*itoa(int num);
// ms_strndup.c
char    *ms_strndup(char *str, int n);
// ms_strncat.c
char    *ms_strncat(char *dst, char *src, int ssize);
// ms_strncpy.c
char    *ms_strncpy(char *dst, char *src, int ssize);
// ms_strchr.c
char    *ms_strchr(char *s, int c);
// loop.c
void    set_handler(int rdline);
void	prompt_loop(void);
// env.c
void	free_env_node_by_key(t_env **head, char *key);
void	set_env_node(t_env **head, char *key, char *value);
t_env	*get_env_node(t_env *head, char *key);
char	**convert_env_to_list(t_env *head);
void	free_env_list(char  **list);
void	free_env(t_env **head);
t_env	*init_env(void);

// parser
// ast_utils.c
int		free_and_null(void **ptr);
void	free_red_args(t_ast *ast, int subtype);
t_ast	*free_ast(t_ast **ast);
t_ast	*make_node(t_ast **ast);
// count_token.c
int		count_token(char *str, t_token *cur, t_token *nxt);
// extract_operator.c
t_ast	*extract_operator(t_ast **ast_nd, char **str, int operator);
// extract_utils.c
int		allocate_ast_args(t_ast *ast, int n_strs);
int		allocate_red_args(t_ast *ast, int n_strs, int subtype);
// print_ast.c
void	print_ast(t_ast *ast, char *testname);
void	print_nd_list(t_ast *ast, char *testname);
// count_cmd_tokens.c
int		chr_after_spaces(t_token *tk);
int		count_cmd_tokens(t_token cur, t_cmd *c);
// extract_cmd.c
int		extract_cmd(char **str, t_parser *s);
// extract_redirect.c
int		skip_red_sign_and_spaces(t_token *cur, int r_subtype);
int		extract_redirect(t_token *cur, t_cmd *c, t_parser *s);
// extract_subshell.c
t_ast	*extract_subshell(t_ast **ast_nd, char **str);
// gen_utils.c
void	ms_bzero(void *s, size_t n);
int		ms_strcmp(char *ref, char *str);
int		ms_strncmp(char *s1, char *s2, int size);
int		ms_strlen(char *str);
int		is_alphanum_or_underscore(char chr);
char	*cat_str_arr(char **dest, char ***address_str_arr, int size);
void	*safe_malloc(void **ptr, size_t size);
// heredoc.c
int 	ms_heredoc(t_ast *ast, t_parser *s);
// parser.c
t_ast	*parser(char *str, t_ast **list_head);
// parser_utils.c
int		type(char *str);
int		subtype(char *str);
char	*ms_strcpy(char *str, int len);
// structure_ast.c
t_ast	*structure_ast(t_ast *cur_list);

// normaliser
// int		normalizer(char **str);

#endif
