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

	output = add(10, 20);
	TEST_ASSERT_EQUAL(30, output);
}

int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_add);
	return (UNITY_END());
}
