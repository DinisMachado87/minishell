#include "../include/minishell.h"

int	skip_count_word(char *str, char limiter)
{
	int	i_ltr;

	i_ltr = 0;
	while (str[i_ltr]
		&& (str[i_ltr] != limiter ||  !is_operator(str + i_ltr)))
	{
		if (limiter == ' ' && (str[i_ltr] == '\'' || str[i_ltr] == '"'))
			limiter = str[i_ltr];
		i_ltr++;
	}
	if (str[i_ltr] && str[i_ltr] == limiter && limiter != ' ')
		i_ltr++;
	return (i_ltr);
}

int	allocate_ast_args(t_ast *ast, int n_strs)
{
	ast->args = malloc((n_strs + 1) * sizeof(char *));
	if (!ast->args)
		return (perror("Err allocating args array"), 0);
	ast->args[n_strs] = 0;
	ast->n_args = n_strs;
	return (1);
}

static int	extract_word_recursive(t_ast *ast, char *str, t_s_parser *s)
{
	int		i_ltr;
	char	*word;
	char	limiter;

	limiter = ' ';
	while (*str && *str == ' ')
		str++;
	i_ltr = skip_count_word(str, limiter);
	if (i_ltr > 0)
		s->i_word++;
	if (str[i_ltr] && !is_operator(str + i_ltr))
		extract_word_recursive(ast, str + i_ltr, s);
	if (!ast->args && !allocate_ast_args(ast, s->i_word))
		return (0);
	word = ms_strcpy(str, i_ltr);
	if (!word)
		return (0);
	ast->args[--s->i_word] = word;
	return(1);
}

t_ast	*extract_cmd(char **str, t_s_parser *s)
{
	t_ast	*ast_nd;

	if (!make_node(&ast_nd))
		return (NULL);
	s->i_word = 0;
	if (!extract_word_recursive(ast_nd, *str, s))
		return (free_all(ast_nd), NULL);
	return (ast_nd);
}
