#include "../include/minishell.h"

void	sig_c_handler(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

char  *get_prompt(void)
{
  char  *cwd;
  size_t  len;
  char  *prompt;

  cwd = getcwd(NULL, 0);
  if (!cwd)
    return (NULL);
  len = strlen(cwd);
  prompt = malloc(len + 3);
  if (!prompt)
  {
    free(cwd);
    return (NULL);
  }
  prompt[0] = '\0';
  strncat(prompt, cwd, len);
  strncat(prompt, "$ ", 3);
  free(cwd);
  return (prompt);
}

/*
 * On EOF readline returns NULL
 */
char  *get_input(char *prompt)
{
  char  *input;

  input = readline(prompt);
  if (!input)
	{
		free(prompt);
    exit(1);
	}
  if (*input)
    add_history(input);
  return (input);
}

/*
 * SIGINT = ctrl_c
 * SIGQUIT = ctrl_\
 */
void	prompt_loop(void)
{
  char  *input;
  char  *prompt;
	struct sigaction	sa_c;

  while (1)
	{
		sa_c.sa_flags = SA_RESTART;
		sigemptyset(&sa_c.sa_mask);
		sa_c.sa_handler = sig_c_handler;
		if (sigaction(SIGINT, &sa_c, NULL) == -1)
			perror("sigaction");
		signal(SIGQUIT, SIG_IGN);
		prompt = get_prompt();
		if (!prompt)
			return ;
		input = get_input(prompt);
		if (!input)
		{
			free(prompt);
			return ;
		}
	}
	if (input)
		free(input);
	free(prompt);
}

int	main(void)
{
	t_ast	*n1;
	t_ast	*n2;
	t_ast	*n3;

	n1 = malloc(sizeof(t_ast));
	n2 = malloc(sizeof(t_ast));
	n3 = malloc(sizeof(t_ast));

  t_shell *shell;
  shell = malloc(sizeof(t_shell));
  shell->ast_tree = n1;
  set_env_node(&shell->env, "PATH", getenv("PATH"));

	n1->type = AND;
	n1->left = n2;
	n1->right = n3;

	n2->type = CMD;
	n2->subtype = ECHO;
	n2->args = malloc(sizeof(char *) * 3);
	n2->args[0] = "echo";
	n2->args[1] = "Hello";
	n2->args[2] = NULL;

	n3->type = CMD;
	n3->subtype = ECHO;
	n3->args = malloc(sizeof(char *) * 3);
	n3->args[0] = "echo";
	n3->args[1] = "World";
	n3->args[2] = NULL;

	execute_ast(shell, n1);
	free(shell);
	free(n1);
	free(n2->args);
	free(n2);
	free(n3->args);
	free(n3);
	return (0);
}

/*
int	main(void)
{
	t_shell	*shell;
	t_ast		*n1;
	t_ast		*n2;
	t_ast		*n3;
	t_ast		*n4;
	t_ast		*n5;

	shell = malloc(sizeof(t_shell));

	n1 = malloc(sizeof(t_ast));
	n2 = malloc(sizeof(t_ast));
	n3 = malloc(sizeof(t_ast));
	n4 = malloc(sizeof(t_ast));
	n5 = malloc(sizeof(t_ast));
	
	//shell->env = malloc(sizeof(t_env));
	shell->ast_tree = n4;

	set_env_node(&shell->env, "PATH", getenv("PATH"));

	n1->type = AND;
	n1->left = n2;
	n1->right = n3;

	n2->type = CMD;
	n2->subtype = CD;
	n2->args = malloc(sizeof(char *) * 3);
	n2->n_args = 2;
	n2->args[0] = "cd";
	n2->args[1] = "..";
	n2->args[2] = NULL;

	n3->type = CMD;
	n3->subtype = ECHO;
	n3->args = malloc(sizeof(char *) * 3);
	n3->args[0] = "echo";
	n3->args[1] = "Hello";
	n3->args[2] = NULL;
	
	n4->type = AND;
	n4->left = n1;
	n4->right = n5;

	n5->type = CMD;
	n5->subtype = CD;
	n5->n_args = 2;
	n5->args = malloc(sizeof(char *) * 3);
	n5->args[0] = "cd";
	n5->args[1] = "..";
	n5->args[2] = NULL;

	execute_ast(shell, n4);

	free(n1);
	free(n2->args);
	free(n2);
	free(n3->args);
	free(n3);
	free(n4);
	free(n5->args);
	free(n5);
	free(shell);
	return (0);
}
*/


/*
int	main(void)
{
	t_ast	*n1;
	t_ast	*n2;
	t_ast	*n3;
	t_ast	*n4;
	t_ast	*n5;
	t_ast	*n6;
	t_ast	*n7;

	n1 = malloc(sizeof(t_ast));
	n2 = malloc(sizeof(t_ast));
	n3 = malloc(sizeof(t_ast));
	n4 = malloc(sizeof(t_ast));
	n5 = malloc(sizeof(t_ast));
	n6 = malloc(sizeof(t_ast));
	n7 = malloc(sizeof(t_ast));

	t_shell *shell;
  shell = malloc(sizeof(t_shell));
  shell->ast_tree = n1;
  set_env_node(&shell->env, "PATH", getenv("PATH"));

	n1->type = AND;
	n1->left = n2;
	n1->right = n3;

	n2->type = PIPE;
	n2->left = n4;
	n2->right = n5;

	n3->type = PIPE;
	n3->left = n6;
	n3->right = n7;

	n4->type = CMD;
	n4->subtype = EXTERNAL;
	n4->args = malloc(sizeof(char *) * 4);
	n4->args[0] = "echo";
	n4->args[1] = "-e";
	n4->args[2] = "1\n2\n3";
	n4->args[3] = NULL;

	n5->type = CMD;
	n5->subtype = EXTERNAL;
	n5->args = malloc(sizeof(char *) * 3);
	n5->args[0] = "grep";
	n5->args[1] = "2";
	n5->args[2] = NULL;

	n6->type = CMD;
	n6->subtype = PWD;
	n6->args = malloc(sizeof(char *) * 2);
	n6->args[0] = "pwd";
	n6->args[1] = NULL;

	n7->type = CMD;
	n7->subtype = EXTERNAL;
	n7->args = malloc(sizeof(char *) * 4);
	n7->args[0] = "tr";
	n7->args[1] = "a-z";
	n7->args[2] = "A-Z";
	n7->args[3] = NULL;

	execute_ast(shell, n1);
	free(n1);
	free(n2);
	free(n3);
	free(n4->args);
	free(n4);
	free(n5->args);
	free(n5);
	free(n6->args);
	free(n6);
	free(n7->args);
	free(n7);
	return (0);
}
*/

/*
int	main(void)
{
	t_ast	*n1;
	t_ast	*n2;
	t_ast	*n3;

	n1 = malloc(sizeof(t_ast));
	n2 = malloc(sizeof(t_ast));
	n3 = malloc(sizeof(t_ast));
	n1->args = malloc(sizeof(char *) * 2);
	n2->args = malloc(sizeof(char *) * 4);
	n1->args[0] = "pwd";
	n1->args[1] = NULL;
	n1->type = CMD;
	n1->subtype = PWD;
	n2->type = CMD;
	n2->subtype = EXTERNAL;
	n2->args[0] = "tr";
	n2->args[1] = "a-z";
	n2->args[2] = "A-Z";
	n2->args[3] = NULL;
	n3->type = PIPE;
	n3->left = n1;
	n3->right = n2;
	execute_ast(n3);
	free(n1->args);
	free(n1);
	free(n2->args);
	free(n2);
	free(n3);
	return (0);
}
*/

/*
int	main(void)
{
	t_ast *n1;
	t_ast	*n2;
	t_ast	*n3;

	n1 = malloc(sizeof(t_ast));
	n2 = malloc(sizeof(t_ast));
	n3 = malloc(sizeof(t_ast));

	n2->args = malloc(sizeof(char *) * 2);
	n3->args = malloc(sizeof(char *) * 4);
	n1->type = PIPE;
	n1->left = n2;
	n1->right = n3;
	n2->type = CMD;
	n2->subtype = PWD;
	n2->args[0] = "pwd";
	n2->args[1] = NULL;
	n3->type = CMD;
	n3->subtype = EXTERNAL;
	n3->args[0] = "tr";
	n3->args[1] = "a-z";
	n3->args[2] = "A-Z";
	n3->args[3] = NULL;
	execute_ast(n1);
	free(n2->args);
	free(n3->args);
	free(n1);
	free(n2);
	free(n3);
	return (0);
}
*/

/*
 * type (1 = pipe; 2 = cmd)
 * subtype (1 = echo, 2 = external)
 * args = e.g. "echo hi"
 */

/*
int	main(void)
{
	t_ast	*n1;

	n1 = malloc(sizeof(t_ast));
	n1->type = CMD;
	n1->subtype = ECHO;
	n1->args = malloc(sizeof(char *) * 5);
	n1->args[0] = "/usr/bin/echo";
	n1->args[1] = "-n";
	n1->args[2] = "Hello World";
	n1->args[3] = "How are you?";
	n1->args[4] = NULL;
	execute_ast(n1);
	free(n1);
	return (0);
}
*/

/*
int	main(void)
{
	t_ast	*n1;
	t_ast	*n2;
	t_ast	*n3;

	n1 = malloc(sizeof(t_ast));
	n2 = malloc(sizeof(t_ast));
	n3 = malloc(sizeof(t_ast));
	n1->type = PIPE;
	n1->left = n2;
	n1->right = n3;
	n2->type = CMD;
	n2->subtype = ECHO;
	n2->args = malloc(sizeof(char *) * 5);
	n2->args[0] = "echo";
	n2->args[1] = "-n";
	n2->args[2] = "Hello World";
	n2->args[3] = "How are you?";
	n2->args[4] = NULL;
	n3->type = CMD;
	n3->subtype = EXTERNAL;
	n3->args = malloc(sizeof(char *) * 2);
	n3->args[0] = "wc";
	n3->args[1] = NULL;
	execute_ast(n1);
	free(n1);
	free(n2->args);
	free(n2);
	free(n3);
	return (0);
}
*/

/*
int	main(void)
{
	t_ast	*n1;
	t_ast	*n2;
	t_ast	*n3;
	t_ast	*n4;
	t_ast	*n5;

	n1 = malloc(sizeof(t_ast));
	n2 = malloc(sizeof(t_ast));
	n3 = malloc(sizeof(t_ast));
	n4 = malloc(sizeof(t_ast));
	n5 = malloc(sizeof(t_ast));

	// |
	n1->type = 1;
	n1->left = n2;
	n1->right = n3;
	// |
	n2->type = 1;
	n2->left = n4;
	n2->right = n5;
	// wc -l
	n3->type = 2;
	n3->subtype = 2;
	n3->args = "wc";
	// echo hi
	n4->type = 2;
	n4->subtype = 1;
	n4->args = "ls";
	// cat
	n5->type = 2;
	n5->subtype = 2;
	n5->args = "cat";
	execute_ast(n1);
	free(n1);
	free(n2);
	free(n3);
	free(n4);
	free(n5);
	return (0);
}
*/

/*
int main(void)
{
	prompt_loop();
  return (0);
}
*/
