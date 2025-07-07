#include "../include/minishell.h"

void  execute_ast(t_ast *node);
void  execute_pipe(t_ast *node);

void	execute_ast(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == 1)
		execute_pipe(node);
	else
		system(node->args);
}

void	execute_pipe(t_ast *node)
{
	int			left_pid;
	int			right_pid;
	int			fd[2];
	int			wstatus;
	
	wstatus = 0;
	if (pipe(fd) < 0)
		return ;
	left_pid = fork();
	if (left_pid < 0)
		return ;
	if (left_pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		execute_ast(node->left);
		exit(0);
	}
	right_pid = fork();
	if (right_pid < 0)
		return ;
	if (right_pid == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		execute_ast(node->right);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(left_pid, &wstatus, 0);
	waitpid(right_pid, &wstatus, 0);
}	

/*
 * type (1 = pipe; 2 = cmd)
 * subtype (1 = echo, 2 = external)
 * args = e.g. "echo hi"
 */
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
	n3->args = "wc -l";
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

/*
void	pipex(void)
{
	int			pid1;
	int			pid2;
	int			fd[2];
	char		*str;
	size_t	n;
	int			wstatus;
	
	str = NULL;
	n = 0;
	wstatus = 0;
	if (pipe(fd) < 0)
		return ;
	pid1 = fork();
	if (pid1 < 0)
		return ;
	if (pid1 == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		system("echo Hello");
		close(fd[1]);
		exit(0);
	}
	pid2 = fork();
	if (pid2 < 0)
		return ;
	if (pid2 == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		getline(&str, &n, stdin);
		close(fd[0]);
		printf("%s\n", str);
		free(str);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &wstatus, 0);
	waitpid(pid2, &wstatus, 0);
}	

void	pipex(t_ast *node)
{
	int			pid1;
	int			pid2;
	int			fd[2];
	int			wstatus;
	
	if (!node->left)
		return ;
	pipex(node->left);
	wstatus = 0;
	if (pipe(fd) < 0)
		return ;
	pid1 = fork();
	if (pid1 < 0)
		return ;
	if (pid1 == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		system(node->left->args);
		close(fd[1]);
		exit(0);
	}
	pid2 = fork();
	if (pid2 < 0)
		return ;
	if (pid2 == 0)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		system(node->right->args);
		close(fd[0]);
		exit(0);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, &wstatus, 0);
	waitpid(pid2, &wstatus, 0);
}	
*/
