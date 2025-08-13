#include "../../include/minishell.h"

char	*ms_strchr(char *s, int c)
{
	while (*s)
	{
		if ((unsigned char)*s == (unsigned char)c)
			return (s);
		s++;
	}
	if ((unsigned char)*s == (unsigned char)c)
		return (s);
	return (NULL);
}
