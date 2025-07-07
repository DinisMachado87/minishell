/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Testparser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlind <jlind@student.42berlin.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 16:26:09 by jlind             #+#    #+#             */
/*   Updated: 2025/07/02 17:10:09 by jlind            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	setUp(void){}

void	tearDown(void){}

void	test_make_node_initializes_list_when_ast_is_null(void)
{
	t_ast	*head = NULL;

	make_node(&head);

	TEST_ASSERT_NOT_NULL(head);
	TEST_ASSERT_NULL(head->next);
}

void	test_make_node_appends_to_existing_list(void)
{
	t_ast	*head = NULL;

	make_node(&head);
	make_node(&head);

	TEST_ASSERT_NOT_NULL(head);
	TEST_ASSERT_NOT_NULL(head->next);
	TEST_ASSERT_NULL(head->next->next);
}

void	test_make_node_creates_three_nodes(void)
{
	t_ast	*head = NULL;
	int		node_count;

	make_node(&head);
	make_node(&head);
	make_node(&head);

	node_count = 0;
	while (head)
	{
		head = head->next;
		node_count++;
	}
	TEST_ASSERT_EQUAL_INT(3, node_count);
}

int	main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_make_node_initializes_list_when_ast_is_null);
	RUN_TEST(test_make_node_appends_to_existing_list);
	RUN_TEST(test_make_node_creates_three_nodes);
	return (UNITY_END());
}
