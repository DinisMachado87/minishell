#include "../include/minishell.h"
#include <stdio.h>

void	free_all(t_ast *ast, t_s_ext_cmd *s)
{
	while (ast->args && --s->i_word >= 0)
		free(ast->args[s->i_word]);
	if (ast->args)
		free(ast->args);
}

int	skip_count_word(char *str, char limiter)
{
	int	i_ltr;

	i_ltr = 0;
	while (str[i_ltr] && str[i_ltr] != limiter)
	{
		if (limiter == ' ' && (str[i_ltr] == '\'' || str[i_ltr] == '"'))
			limiter = str[i_ltr];
		i_ltr++;
	}
	if (str[i_ltr] && str[i_ltr] == limiter && limiter != ' ')
		i_ltr++;
	return (i_ltr);
}

static int	extract_word_recursive(t_ast *ast, char *str, t_s_ext_cmd *s)
{
	int		i_ltr;
	char	*word;
	char	limiter;

	limiter = ' ';
	while (*str && *str == ' ')
		str++;
	i_ltr = skip_count_word(str, limiter);
	s->i_word++;
	if (str[i_ltr])
		extract_word_recursive(ast, str + i_ltr, s);
	if (!ast->args)
	{
		ast->args = malloc((s->i_word + 1) * sizeof(char *));
		if (!ast->args)
			return (perror("Err allocating args array"), ERROR);
		ast->args[s->i_word] = NULL;
	}
	word = malloc((i_ltr + 1) * sizeof(char));
	if (!word)
		return (perror("Err extracting word"), ERROR);
	word[i_ltr] = '\0';
	while (0 <= --i_ltr)
		word[i_ltr] = str[i_ltr];
	ast->args[--s->i_word] = word;
	return(0);
}

int	extract_cmd(t_ast *ast, char *str)
{
	t_s_ext_cmd	s;
	
	s.i_word = 0;
	s.error = 0;
	s.error = extract_word_recursive(ast, str, &s);
	if (s.error)
		return (free_all(ast, &s), s.error);
	return (0);
}
