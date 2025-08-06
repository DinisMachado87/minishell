#include "../include/minishell.h"

const char *type_str[] = {
	"SUBSHELL",
	"CMD",
	"REDIRECT",
	"PIPE",
	"OPERATOR",
};

const char *subtype_str[] =
{
	"NONE",
	"EXTERNAL",
	"VAR",
	"ECHO",
	"CD",
	"PWD",
	"EXPORT",
	"UNSET",
	"ENV",
	"EXIT",
	"OR",
	"AND",
};

const char *redirect_subtype_str[] = {
	"IN",
	"OUT",
	"APPEND",
	"HEREDOC",
};

static void	print_indent(int indent)
{
	while (indent)
	{
		printf("\t");
		indent--;
	}
}

static void print_ast_str(char *name, char *element, int indent, char *new_line)
{
	print_indent(indent);
	if (element)
		printf("%s:%s", name, element);
	else
		printf("%s:NONE", name);
	printf("%s", new_line);
};

static void print_ast_arr(char **arr, int indent)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		print_indent(indent + 1);
		print_ast_str("", arr[i], indent, "\n");
		i++;
	}
};

static void	print_node_type(t_ast *node, char *name, int indent, char *new_line)
{
	print_indent(indent);
	if (node)
		printf("%s:%s\t", name, type_str[node->type]);
	else
		printf("%s:NONE\t", name);
	printf("%s", new_line);
}

static void	print_ast_nd(t_ast *ast, char *testname, int indent)
{
	if (testname)
		printf("== %s ==\n\n", testname);
	print_indent(indent);
	printf("TYPE:%s\t", type_str[ast->type]);
	print_ast_str("SUBTYPE", (char *)type_str[ast->type], indent, "\n");
	print_indent(indent);
	printf("N_ARGS:\t%d\n", ast->n_args);
	print_ast_arr(ast->args, indent);
	print_ast_str("RED_IN", ast->red_args[IN], indent, "\t");
	print_ast_str("RED_OUT", ast->red_args[OUT], indent, "\n");
	print_node_type(ast->next, "NEXT", indent, "");
	print_node_type(ast->left, "LEFT", indent, "");
	print_node_type(ast->right, "RIGHT", indent, "\n");
	printf("\n");
}


void	print_nd_list(t_ast *ast, char *testname)
{
	while (ast)
	{
		print_ast_nd(ast, testname, 0);
		ast = ast->next;
	}
}

void	print_ast_core(t_ast *ast, int *indent)
{
	while (ast)
	{
		if (ast->left)
			print_ast_core(ast->left, indent);
		print_ast_nd(ast, 0, *indent);
		if (ast->right)
			print_ast_core(ast->right, indent);

	}
}

void	print_ast(t_ast *ast, char *testname)
{
	int	indent;

	indent = 0;
	printf("== %s ==\n\n", testname);
	print_ast_core(ast, &indent);
}
