#include "../../include/minishell.h"

char	*ms_strndup(char *str, int n)
{
	int	len;
	char	*dup;
	int	i;

	i = 0;
	if ((int)n < 0)
		n = 0;
	len = ms_strlen(str);
	if (n < len)
		len = n;
	dup = malloc(len + 1);
	if (!dup)
		return (NULL);
	dup[len] = '\0';
	while (i < len)
	{
		dup[i] = *(str + i);
		i++;
	}
	return (dup);
}
