#include "../../include/minishell.h"

char	*ms_strncat(char *dst, char *src, size_t ssize)
{
	char	*ret;

	ret = dst;
	while (*dst)
		dst++;
	while (*src && ssize--)
		*dst++ = *src++;
	*dst = '\0';
	return (ret);
}
