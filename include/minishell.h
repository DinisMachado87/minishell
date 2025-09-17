/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:31:32 by dimachad          #+#    #+#             */
/*   Updated: 2025/09/17 17:36:37 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define ERROR -1
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
# include <dirent.h>

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

typedef struct s_args {
	char	**tkns;
	int		*exp;
	int		*space;
	int		*type;
	int		n;
}	t_args;

typedef struct s_ast
{
	int				type;
	int				subtype;
	t_args			args[3];
	struct s_ast    *next;
	struct s_ast    *left;
	struct s_ast    *right;
}	t_ast;

typedef struct  s_state_parser
{
	t_ast	*ast;
	int		n_heredoc;
}			t_parser;

typedef struct s_state_token
{
	char	limiter;
	int		space_after;
	int		expand;
	char	*str;
}	t_token;

typedef struct s_state_cmd {
	int	n_cmd_tk;
	int	i_tkn;
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
	t_ast   *list;
	t_ast   *ast;
	t_env   *env;
	int		exit_status;
}			t_shell;

// normaliser
int		normalizer(char **str);
// str_pairs_even
int		str_pairs_even(char *str);
// parser
int		parser(char *str, t_shell *sh);
// cmd_expander
int		cmd_expander(t_ast *ast, t_shell *sh);
// ast_utils
t_ast	*make_node(t_ast **ast);
t_ast	*free_ast(t_ast **ast);
void	free_and_null_str_arr(char ***address_str_arr);
int		free_and_null(void **ptr);
// parser_utils
int		type(char *str);
int		subtype(char *str);
int		strict_subtype(char *str);
char	*ms_strcpy(char *str, int len);
// extract_utils
void	*safe_alloc_zero(void **ptr, size_t size);
void	*safe_malloc(void **ptr, size_t size);
int		allocate_ast_args(t_ast *ast, int n_strs);
int		allocate_red_args(t_ast *ast, int n_strs, int subtype);
// gen_utils
void	ms_bzero(void *s, size_t n);
int		ms_strcmp(char *ref, char *str);
int		ms_strncmp(char *s1, char *s2, int size);
int		ms_strlen(char *str);
int		is_alphanum_or_underscore(char chr);
char	*cat_str_arr(char **dest, char **str_arr, int size);
// extract cmd
int		chr_after_spaces(t_token *tk);
int		count_token(char *str, t_token *cur, t_token *nxt);
int		count_redirect(t_token cur, t_cmd *c, t_ast *ast);
int		count_cmd_tokens(t_token cur, t_cmd *c);
void	free_red_args(t_ast *ast, int subtype);
int		extract_cmd(char **str, t_parser *s);
int		skip_red_sign_and_spaces(t_token *cur, int r_subtype);
t_ast	*extract_subshell(t_ast **ast_nd, char **str);
t_ast	*extract_operator(t_ast **ast_nd, char **str, int operator);
int		extract_redirect(t_token *cur, t_cmd *c, t_parser *s);
int 	ms_heredoc(t_ast *ast, t_parser *s);
// structure_ast
t_ast	*structure_ast(t_ast *cur_list);
// print_ast
void	print_ast(t_ast *ast, char *testname);
void	print_nd_list(t_ast *ast, char *testname);
// prompt_loop
void	prompt_loop(char *envp[]);
void	execute_ast(t_shell *shell, t_ast *node);
void	execute_pipe(t_shell *shell, t_ast *node);
int		ft_echo(t_ast *node);
int		ft_cd(t_shell *shell, t_ast *node);
int		ft_pwd(void);
int		ft_export(t_shell *shell, t_ast *node);
int		ft_unset(t_shell *shell, t_ast *node);
int		ft_env(t_shell *shell, t_ast *node);
void	ft_exit(t_shell *shell, t_ast *node);
void	execute_and(t_shell *shell, t_ast *node);
void	execute_or(t_shell *shell, t_ast *node);
char    *get_cmd_path(t_shell *shell, char *cmd);
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
char    *ms_strndup(char *str, int n);
void    init_env(t_shell *shell, char *envp[]);
char    *ms_strncat(char *dst, char *src, int ssize);
char    *ms_strncpy(char *dst, char *src, int ssize);
char    *ms_strchr(char *s, int c);
char	*ms_strrchr(char *s, int c);
int		ms_atoi(char *nbr);
int		ms_isalpha(char *str);
int		ms_char_isalpha(char c);
int		ms_isdigit(char *str);
int		ms_char_isdigit(char c);
int		is_valid_identifier(char *str);
void    set_handler(int rdline);
void	free_shell(t_shell *shell);

#endif
