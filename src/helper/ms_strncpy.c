#include "../../include/minishell.h"

char	*ms_strncpy(char *dst, char *src, int ssize)
{
	char	*ret;

	ret = dst;
	while (ssize && *src )
	{
		*dst++ = *src++;
		ssize--;
	}
	while (ssize--)
		*dst++ = '\0';
	return (ret);
}
