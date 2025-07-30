#include "../include/minishell.h"

static int	skip_count_word(char *str, char limiter)
{
	int	i_ltr;

	i_ltr = 0;
	while (str[i_ltr]
		&& (str[i_ltr] != limiter && !is_operator(str + i_ltr)))
	{
		if (limiter == ' ' && (str[i_ltr] == '\'' || str[i_ltr] == '"'))
			limiter = str[i_ltr];
		i_ltr++;
	}
	if (str[i_ltr] && str[i_ltr] == limiter && limiter != ' ')
		i_ltr++;
	return (i_ltr);
}

static int	extract_word_recursive(t_ast *ast, char *str, t_s_parser *s)
{
	int		i_ltr;
	int		spaces;
	char	*word;

	spaces = 0;
	i_ltr = 0;
	while (str[spaces] && str[spaces] == ' ')
		spaces++;
	if (!str[spaces] || str[spaces] == '(')
		return (spaces);
	i_ltr = skip_count_word((str + spaces), ' ');
	s->i_word++;
	if (str[spaces + i_ltr] && !is_operator(str + spaces + i_ltr))
		s->n_cmd_ltrs += extract_word_recursive(ast, str + spaces + i_ltr, s);
	if (!ast->args && !allocate_ast_args(ast, s->i_word))
		return (0);
	if (str[spaces] == '\'' || str[spaces] == '\"')
		word = ms_strcpy((str + spaces + 1), i_ltr - 2);
	else
		word = ms_strcpy((str + spaces), i_ltr);
	if (!word)
		return (0);
	ast->args[--s->i_word] = word;
	return(spaces + i_ltr);
}

t_ast	*extract_cmd(t_ast **ast_nd, char **str, t_s_parser *s)
{
	s->i_word = 0;
	s->n_cmd_ltrs = 0;
	if (!make_node(ast_nd))
		return (NULL);
	s->n_cmd_ltrs += extract_word_recursive(*ast_nd, *str, s);
	if (!s->n_cmd_ltrs)
		return (free_all(*ast_nd), NULL);
	(*ast_nd)->type = CMD;
	*str += s->n_cmd_ltrs;
	return (*ast_nd);
}
