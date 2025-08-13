#include "../../include/minishell.h"

size_t ms_strlen(char *str)
{
	size_t	len;

	len = 0;
	while (*(str + len))
		len++;
	return (len);
}
