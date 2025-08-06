#include "../include/minishell.h"

void	setUp(void){}

void	tearDown(void){}

void	test_echo_with_newline(void)
{
	int pipefd[2];
	char buffer[1024];
	int saved_stdout;
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
	n2->args = malloc(sizeof(char *) * 3);
	n2->args[0] = "echo";
	n2->args[1] = "hello";
	n2->args[2] = NULL;

	n3->type = CMD;
	n3->subtype = EXTERNAL;
	n3->args = malloc(sizeof(char *) * 3);
	n3->args[0] = "wc";
	n3->args[1] = "-c";
	n3->args[2] = NULL;

	saved_stdout = dup(STDOUT_FILENO);
	pipe(pipefd);
	dup2(pipefd[1], STDOUT_FILENO);
	execute_ast(n1);
	dup2(saved_stdout, STDOUT_FILENO);
	close(pipefd[1]);
	read(pipefd[0], buffer, sizeof(buffer));
	TEST_ASSERT_EQUAL_STRING("6\n", buffer);
	free(n2->args);
	free(n2);
	free(n3->args);
	free(n3);
	free(n1);
}

int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_echo_with_newline);
	return (UNITY_END());
}

