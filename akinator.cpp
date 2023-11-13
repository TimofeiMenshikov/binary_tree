#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/bin_tree.h"
#include "include/text.h"

const ssize_t MAX_DYNAMIC_PTRS = 1000;

struct Names_with_dynamic_mem 
{
	char* arr[MAX_DYNAMIC_PTRS] = {};
	ssize_t size = 0;
};

struct Names_with_dynamic_mem names_with_dynamic_mem;

struct Bin_tree_elem* ask_question(struct Bin_tree_elem* elem_ptr)
{
	printf("%s?\n", elem_ptr->elem);

	bool is_right = false;

	scanf("%d", &is_right);

	if (is_right)
	{
		struct Bin_tree_elem* return_elem = elem_ptr->right_child_ptr;
		return return_elem;
	}

	struct Bin_tree_elem* return_elem = elem_ptr->left_child_ptr;
	return return_elem;
}

err_t akinator(struct Bin_tree* const bin_tree_ptr)
{
	CHECK_BIN_TREE();

	struct Bin_tree_elem* current_elem_ptr = bin_tree_ptr->root_ptr;

	while ((current_elem_ptr->left_child_ptr != NULL) && (current_elem_ptr->right_child_ptr != NULL))
	{
		current_elem_ptr = ask_question(current_elem_ptr);
		printf("after ask\n");
		printf("[%p]\n", current_elem_ptr);
	} 

	printf("you are %s?\n", current_elem_ptr->elem);

	bool is_akinator_right = false;

	scanf("%d", &is_akinator_right);

	if (is_akinator_right)
	{
		printf("Easy\n");
	}
	else
	{
		printf("what is it?\n");
		char* new_name = (char*) calloc(MAX_STR_SIZE, sizeof(char));
		(names_with_dynamic_mem.arr)[names_with_dynamic_mem.size] = new_name;
		(names_with_dynamic_mem.size)++;


		scanf("%s", new_name);

		printf("how is %s different from %s?\n", new_name, current_elem_ptr->elem);

		char* new_question = (char*) calloc(MAX_STR_SIZE, sizeof(char));
		scanf("%s", new_question);

		(names_with_dynamic_mem.arr)[names_with_dynamic_mem.size] = new_question;
		(names_with_dynamic_mem.size)++;

		printf("OK\n");

		init_bin_tree_elem(bin_tree_ptr, current_elem_ptr, LEFT_CHILD, current_elem_ptr->elem);
		init_bin_tree_elem(bin_tree_ptr, current_elem_ptr, RIGHT_CHILD, new_name);

		current_elem_ptr->elem = new_question;
	}

	return return_code;
}

static ssize_t find_id_by_name(struct Bin_tree_elem* const elem_ptr, const char* const name, const ssize_t name_size, ssize_t* const depth_ptr)
{
	ssize_t id = 0;

	if (elem_ptr == NULL)
	{
		return 0;
	}

	if (!strncmp(elem_ptr->elem, name, name_size))
	{
		id = elem_ptr->id;

		if (*depth_ptr == 0)
		{
			*depth_ptr = elem_ptr->depth_in_tree;
		}
		else 
		{
			printf("error 2 or more matches\n");
		}

	}
	else
	{
		id += find_id_by_name(elem_ptr->left_child_ptr, name, name_size, depth_ptr);
		id += find_id_by_name(elem_ptr->right_child_ptr, name, name_size, depth_ptr);
	}


	return id;
}


err_t find_all_signes(struct Bin_tree* const bin_tree_ptr, const char* const name, ssize_t name_size)
{
	ssize_t depth = 0;

	ssize_t id = find_id_by_name(bin_tree_ptr->root_ptr, name, name_size, &depth);

	struct Bin_tree_elem* current_elem_ptr = bin_tree_ptr->root_ptr;

	for (ssize_t i = depth - 2; i >= 0; i--)
	{
		if (id & 1 << i)
		{
			printf("%s\n", current_elem_ptr->elem);
			current_elem_ptr = current_elem_ptr->right_child_ptr;
		}
		else
		{
			printf("NOT %s\n", current_elem_ptr->elem);
		}
	}

	return NO_ERROR;
}


err_t compare_two_names(struct Bin_tree* const bin_tree_ptr, const char* const name1, ssize_t name1_size, const char* const name2, ssize_t name2_size)
{
	ssize_t depth1 = 0;
	ssize_t depth2 = 0;

	ssize_t id1 = find_id_by_name(bin_tree_ptr->root_ptr, name1, name1_size, &depth1);
	ssize_t id2 = find_id_by_name(bin_tree_ptr->root_ptr, name2, name2_size, &depth2);

	struct Bin_tree_elem* current_elem_ptr1 = bin_tree_ptr->root_ptr;

	ssize_t min_depth = 0;

	if (depth1 < depth2)
	{
		min_depth = depth1;
	}
	else 
	{
		min_depth = depth2;
	}

	ssize_t i = 0;

	

	printf("similar:\n");

	while (i <= min_depth - 2)
	{
		if ((id1 & (1 << (depth1 - 2 - i))) && (id2 & (1 << (depth2 - 2 - i))))
		{
			printf("%s\n", current_elem_ptr1->elem);
			current_elem_ptr1 = current_elem_ptr1->right_child_ptr;
			i++;
		}
		else if (!(id1 & (1 << (depth1 - 2 - i))) && !(id2 & (1 << (depth2 - 2 - i))))
		{
			printf("NOT %s\n", current_elem_ptr1->elem);
			current_elem_ptr1 = current_elem_ptr1->left_child_ptr;
			i++;
		}
		else break;
	}

	printf("differences:\n");

	struct Bin_tree_elem* current_elem_ptr2 = current_elem_ptr1;

	printf("%s\n", name1);

	for (ssize_t j = i; j <= depth1 - 2; j++)
	{
		if (id1 & (1 << (depth1 - 2 - i)))
		{
			printf("%s\n", current_elem_ptr1->elem);
			current_elem_ptr1 = current_elem_ptr1->right_child_ptr;
		}
		else
		{
			printf("NOT %s\n", current_elem_ptr1->elem);
			current_elem_ptr1 = current_elem_ptr1->left_child_ptr;
		}
	}

	printf("%s\n", name2);

	for (ssize_t j = i; j <= depth2 - 2; j++)
	{
		if (id2 & (1 << (depth2 - 2 - i)))
		{
			printf("%s\n", current_elem_ptr2->elem);
			current_elem_ptr2 = current_elem_ptr2->right_child_ptr;
		}
		else
		{
			printf("NOT %s\n", current_elem_ptr2->elem);
			current_elem_ptr2 = current_elem_ptr2->left_child_ptr;
		}
	}

	return NO_ERROR;
}