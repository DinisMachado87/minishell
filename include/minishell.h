/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 15:31:32 by dimachad          #+#    #+#             */
/*   Updated: 2025/10/09 23:41:43 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SUCCESS 0
# define ERROR -1
# define FATAL -2
# define SYNTAX -3
# define TEMP_PREFIX "heredoc"

# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

enum					e_new
{
	OLD,
	NEW,
};

enum					e_expand
{
	DONT_EXPAND,
	EXPAND,
	POTENCIALLY_EXPAND,
};

enum					e_space
{
	NO_SPACE_AFTER,
	SPACE_AFTER,
};

typedef enum e_type
{
	SUBSHELL,
	CMD,
	REDIRECT,
	PIPE,
	OPERATOR,
}						t_type;

typedef enum e_subtype
{
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
}						t_subtype;

typedef enum e_redirect_subtype
{
	IN = 1,
	OUT,
	HEREDOC,
	APPEND,
}						t_redirect_subtype;

typedef struct s_token
{
	char				*str;
	int					type;
	int					subtype;
}						t_s_token;

static const t_s_token	g_types[] = {
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
{"\(", SUBSHELL, NONE},
{0, 0, 0},
};

typedef struct s_args
{
	char				**tkns;
	int					*exp;
	int					*space;
	int					*type;
	int					n;
}						t_args;

typedef struct s_ast
{
	int					type;
	int					subtype;
	t_args				args[3];
	struct s_ast		*next;
	struct s_ast		*left;
	struct s_ast		*right;
}						t_ast;

typedef struct s_state_parser
{
	t_ast				*ast;
	int					n_heredoc;
}						t_parser;

typedef struct s_state_token
{
	char				limiter;
	int					space_after;
	int					expand;
	char				*str;
}						t_token;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_shell
{
	t_ast				*list;
	t_ast				*ast;
	t_env				*env;
	int					exit_status;
	char				**input;
}						t_shell;

typedef struct s_state_cmd
{
	int					n_cmd_tk;
	int					i_tkn;
	int					ltr;
	int					type;
	int					subtype;
	t_shell				*sh;
	t_ast				*cur;
}						t_cmd;

// normaliser
int						normalizer(char **str);
// str_pairs_even
int						str_pairs_even(char *str);
// parser
int						parser(char *str, t_shell *sh, t_ast *cur);
// cmd_expander
int						cmd_expander(t_args *args, t_shell *sh);
char					*unique_tmp(char **dest, char *str1, char *str2);
// ast_utils
int						is_space(char c);
t_ast					*make_node(t_ast **ast);
t_ast					*free_ast(t_shell *sh);
t_ast					*free_ast_in_heredoc_fork(t_shell *sh);
void					free_and_null_str_arr(char ***address_str_arr);
int						free_and_null(void **ptr);
// parser_utils
int						type(char *str);
int						subtype(char *str);
int						strict_subtype(char *str);
int						strict_cmp(char *ref, char *str);
char					*ms_strcpy(char *str, int len);
// extract_utils
int						psynterr(char *str, t_shell *sh);
void					*safe_alloc_zero(void **ptr, size_t size);
void					*safe_malloc(void **ptr, size_t size);
int						allocate_ast_args(t_args *args, int n_strs);
void					*safe_alloc_zero(void **ptr, size_t size);
// gen_utils
void					ms_bzero(void *s, size_t n);
int						ms_strcmp(char *ref, char *str);
int						ms_strncmp(char *s1, char *s2, int size);
int						ms_strlen(char *str);
int						is_alphanum_or_underscore(char chr);
char					*cat_str_arr(char **dest, char **str_arr, int size);
// extract cmd
int						chr_after_spaces(t_token *tk);
int						count_token(char *str, t_token *cur, t_token *nxt);
int						count_redirect(t_token cur, t_args *args);
int						count_cmd_tokens(t_token cur, t_cmd *c, char **err_str);
void					handle_end_quote(char *str, t_token *cur,
							t_token *nxt, int *i_ltr);
int						open_fds(int *old_fd, int *new_fd,
							char *filename, char *exp_file);
int						free_and_null_args(t_args *args);
int						free_and_reassign(char **dest, char **src);
int						extract_cmd(char **str, t_ast **lst_nd, t_shell *sh);
int						tkns_to_words(t_args *args, int alloc_len);
int						skip_red_sign_and_spaces(t_token *cur, int r_subtype);
t_ast					*extract_subshell(t_ast **ast_nd, char **str);
t_ast					*extract_operator(t_ast **ast_nd, char **str,
							int operator);
int						extract_redirect(t_token *cur, t_cmd *c,
							t_args *new_args, t_args old_args);
int						expand_exit_status(char **tkn, t_shell *sh);
int						expand_heredoc(char **filename, t_shell *sh);
void					set_and_save_signal(int signal, void (*handler)(int),
							struct sigaction *old_signal);
int						ms_heredoc(t_args *args, int offset,
							int expand, t_cmd *c);
int						wait_to_store_file(pid_t pid,
							char **temp_file, char **arg_tkn);
int						create_heredoc_file(char *eof,
							char *temp_file, t_cmd *c);
//	expand_tkn_arr
int						expand_tkn_arr(t_args *args, t_shell *sh);
// split_cmd_flags
int						split_cmd_flags(char *str, t_args *args, int alloc_len);
// structure_ast
t_ast					*structure_ast(t_ast *cur_list);
// print_ast
void					print_ast(t_ast *ast, char *testname);
void					print_nd_list(t_ast *ast, char *testname);
void					print_args_arr(char *print_str, t_args *args,
							int indent);
// print_utils
void					print_ast_nd(t_ast *ast, char *testname, int indent);
// prompt_loop
int						prompt_loop_wrapper(char *envp[]);
void					execute_ast(t_shell *shell, t_ast *node);
void					execute_pipe(t_shell *shell, t_ast *node);
int						ms_echo(t_ast *node);
int						ms_cd(t_shell *shell, t_ast *node);
int						ms_pwd(void);
int						ms_export(t_shell *shell, t_ast *node);
int						ms_unset(t_shell *shell, t_ast *node);
int						ms_env(t_shell *shell, t_ast *node);
void					ms_exit(t_shell *shell, t_ast *node);
void					execute_and(t_shell *shell, t_ast *node);
void					execute_or(t_shell *shell, t_ast *node);
char					*get_cmd_path(t_shell *shell, char *cmd);
char					*global_cmd_valid(t_shell *shell, char *cmd);
char					*local_cmd_valid(t_shell *shell, char *cmd);
t_env					*gen_env_node(char *key, char *value);
void					free_env_node(t_env **node);
void					free_env_node_by_key(t_env **head, char *key);
void					set_env_node(t_env **head, char *key, char *value);
t_env					*get_env_node(t_env *head, char *key);
int						get_env_length(t_env *head);
char					*gen_env_str(char *key, char *value);
char					**convert_env_to_list(t_env *head);
void					free_env(t_env **head);
void					print_env(t_shell *shell, int export);
void					update_env_node(t_env *env, char *key, char *value);
void					cleanup(t_shell **shell);
void					free_env_list(char **list);
int						print_err(char *cmd_name, char *err_msg, int err);
char					*itoa(int num);
char					*ms_strndup(char *str, int n);
void					init_env(t_shell *shell, char *envp[]);
char					*ms_strncat(char *dst, char *src, int ssize);
char					*ms_strncpy(char *dst, char *src, int ssize);
char					*ms_strchr(char *s, int c);
char					*ms_strrchr(char *s, int c);
int						ms_atoi(char *nbr);
int						ms_isalpha(char *str);
int						ms_char_isalpha(char c);
int						ms_isdigit(char *str);
int						ms_char_isdigit(char c);
int						is_valid_identifier(char *str);
void					set_handler(int rdline);
void					free_shell(t_shell *shell);
int						print_sys_err(char *syscall, char *component,
							char *err_msg, int err);
int						get_input(char *prompt, char **input, int *eof);
void					execute_external(t_shell *shell, t_ast *node);
void					execute_cmd(t_shell *shell, t_ast *node);
int						setup_red_in(char *file, int type);
int						setup_red_out_wrapper(char *file, int type);
int						execute_built_in(t_shell *shell, t_ast *node);

#endif
