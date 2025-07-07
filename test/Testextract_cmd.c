#include "../test/tests.h"
#include "../include/minishell.h"

void	setUp(void){}

void	tearDown(void){}

void	test_make_node_initializes_list_when_ast_is_null(void)
{
	t_ast	*ast = NULL;
	char	*str = "echo the city is wilder than you think";
	char	*str2 = "echo 'the city is wilder than you think'";

	make_node(&ast);

	TEST_ASSERT_NOT_NULL(ast);
	TEST_ASSERT_NULL(ast->next);

	extract_cmd(ast, str);
}

int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_make_node_initializes_list_when_ast_is_null);
	return (UNITY_END());
}
