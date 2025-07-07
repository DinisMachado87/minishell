#include "../include/minishell.h"

void	setUp(void){}

void	tearDown(void){}

void	test_arr_cmp(void)
{
	char	*arr1[] = {"Hello", "Bye"};
	char	*arr2[] = {"Hello", "Bye"};

	TEST_ASSERT_EQUAL_STRING_ARRAY(arr1, arr2, 2);
}

int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_arr_cmp);
	return (UNITY_END());
}
