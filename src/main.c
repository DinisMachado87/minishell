#include "../include/minishell.h"

int	main(void)
{
	prompt_loop();
}

/*
int	main(void)
{
	t_ast		*n1;
	t_ast		*n2;
	t_ast		*n3;
	t_shell	*shell;
	t_env		*env;

	env = NULL;
	set_env_node(&env, "PATH", "/usr/bin");

	n1 = malloc(sizeof(t_ast));
	n2 = malloc(sizeof(t_ast));
	n3 = malloc(sizeof(t_ast));

	n1->next = n2;
	n2->next = n3;

	shell = malloc(sizeof(t_shell));

	shell->ast_tree = n1;
	shell->ast_head = n1;
	shell->env = env;

	n1->type = AND;
	n1->args = NULL;
	n1->left = n2;
	n1->right = n3;

	n2->type = CMD;
	n2->subtype = ECHO;
	n2->args = malloc(sizeof(char *) * 3);
	n2->args[0] = "echo";
	n2->args[1] = "hello";
	n2->args[2] = NULL;
	n2->n_args = 2;

	n3->type = CMD;
	n3->subtype = ECHO;
	n3->args = malloc(sizeof(char *) * 3);
	n3->args[0] = "echo";
	n3->args[1] = "bye";
	n3->args[2] = NULL;
	n3->n_args = 2;

	execute_ast(shell, n1);
	free_all(&shell->ast_head);
	return (0);
}
*/
