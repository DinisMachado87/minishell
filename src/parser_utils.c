#include "../include/minishell.h"

int	is_operator(char *str)
{
	if (*str == '&' && *(str + 1) == '&')
		return (AND);
	if (*str == '|' && *(str + 1) == '|')
		return (OR);
	if (*str == '|')
		return (PIPE);
	if (*str == '>' && *(str + 1) == '>')
		return (APPEND);
	if (*str == '<' && *(str + 1) == '<')
		return (HEREDOC);
	if (*str == '>')
		return (REDIRECT_OUT);
	if (*str == '<')
		return (REDIRECT_IN);
	return (0);
}

void	*handle_error(char *err_msg, int *error)
{
	*error = -1;
	perror(err_msg);
	return (0);
}

char	*ms_strcpy(char *str, int len)
{
	char	*substring;

	substring = malloc((len + 1) * sizeof(char));
	if (!substring)
		return (perror("Err extracting substring"), NULL);
	substring[len] = '\0';
	while (0 <= --len)
		substring[len] = str[len];
	return (substring);
}
