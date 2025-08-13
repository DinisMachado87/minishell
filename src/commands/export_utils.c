#include "../../include/minishell.h"

/*
 * int export => if 1, basic formatting, else env formatting
 */
void	print_env(t_shell *shell, int export)
{
	(void)export;
	t_env	*var;

	var = shell->env;
	while (var)
	{
		if (export)
			printf("declare -x %s=%s\n", var->key, var->value);
		else
			printf("%s=%s\n", var->key, var->value);
		var = var->next;
	}
}
