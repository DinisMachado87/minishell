#include "../include/calc.h"

void	setUp()
{
}

void	tearDown()
{
}

void	test_add()
{
	int	output;

	output = add(20, 20);
	TEST_ASSERT_EQUAL(30, output);
}

int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_add);
	UNITY_END();
	return (0);
}
