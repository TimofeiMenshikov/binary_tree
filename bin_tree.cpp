#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "include/bin_tree.h"
#include "include/graph.h"
#include "include/text.h"

struct Bin_tree_elem* init_bin_tree_elem(struct Bin_tree* const bin_tree_ptr, struct Bin_tree_elem* const parent_ptr, const bool left_or_right, const elem_t elem) // left - 0 right - 1
{
	struct Bin_tree_elem* current_elem_ptr = (struct Bin_tree_elem*) calloc(1, sizeof(struct Bin_tree_elem));

	if (parent_ptr != NULL)
	{	
		if (left_or_right == LEFT_CHILD)
		{
			parent_ptr->left_child_ptr = (struct Bin_tree_elem*) calloc(1, sizeof(struct Bin_tree_elem));
			current_elem_ptr = parent_ptr->left_child_ptr;
		}
		else 
		{
			parent_ptr->right_child_ptr = (struct Bin_tree_elem*) calloc(1, sizeof(struct Bin_tree_elem));
			current_elem_ptr = parent_ptr->right_child_ptr;
		}

		current_elem_ptr->depth_in_tree = (parent_ptr->depth_in_tree) + 1;
	}
	else 
	{
		return NULL;
	}

	current_elem_ptr->right_child_ptr = NULL;
	current_elem_ptr->left_child_ptr = NULL;
	current_elem_ptr->elem = elem;

	(current_elem_ptr->id) = 2 * (parent_ptr->id) + left_or_right;

	(bin_tree_ptr->tail_ptr)->next_ptr = current_elem_ptr;
	current_elem_ptr->prev_ptr = bin_tree_ptr->tail_ptr;
	current_elem_ptr->next_ptr = NULL;

	bin_tree_ptr->tail_ptr = current_elem_ptr;


	(bin_tree_ptr->size)++;

	return current_elem_ptr;
}


err_t init_bin_tree(struct Bin_tree* const bin_tree_ptr, const elem_t first_elem = NULL)
{
	err_t return_code = NO_ERROR;

	bin_tree_ptr->root_ptr = (struct Bin_tree_elem*) calloc(1, sizeof(struct Bin_tree_elem));

	(bin_tree_ptr->root_ptr)->right_child_ptr = NULL;
	(bin_tree_ptr->root_ptr)->left_child_ptr = NULL;

	printf("%s\n", first_elem);

	(bin_tree_ptr->root_ptr)->elem = first_elem;
	(bin_tree_ptr->root_ptr)->depth_in_tree = 1;

	printf("%s\n", (bin_tree_ptr->root_ptr)->elem);

	bin_tree_ptr->head_ptr = bin_tree_ptr->root_ptr;
	bin_tree_ptr->tail_ptr = bin_tree_ptr->root_ptr;

	(bin_tree_ptr->root_ptr)->prev_ptr = NULL;
	(bin_tree_ptr->root_ptr)->next_ptr = NULL;

	(bin_tree_ptr->root_ptr)->id = 1;

	bin_tree_ptr->size = 1;

	return return_code;
}


err_t bin_tree_verificator(const struct Bin_tree* const bin_tree_ptr)
{
	err_t return_code = NO_ERROR;

	if ((bin_tree_ptr->root_ptr) == NULL) return_code |= ROOT_PTR_IS_NULL;
	if ((bin_tree_ptr->head_ptr) == NULL) return_code |= HEAD_PTR_IS_NULL;
	if ((bin_tree_ptr->tail_ptr) == NULL) return_code |= TAIL_PTR_IS_NULL;
	if ((bin_tree_ptr->size) <= 0) return_code |= INVALID_BIN_TREE_SIZE; 

	return return_code;
}


err_t print_bin_tree_err(const err_t return_code)
{
	if (return_code & ROOT_PTR_IS_NULL)
	{
		fprintf(stderr, "root element pointer is NULL\n");
	} 
	if (return_code & HEAD_PTR_IS_NULL)
	{
		fprintf(stderr, "head pointer is NULL\n");
	} 
	if (return_code & TAIL_PTR_IS_NULL)
	{
		fprintf(stderr, "tail pointer is NULL\n");
	}
	if (return_code & INVALID_BIN_TREE_SIZE)
	{
		fprintf(stderr, "invalid bin tree size\n");
	} 

	return return_code;
}


err_t print_bin_tree_elem(const struct Bin_tree_elem* const elem_ptr)
{
	err_t return_code = NO_ERROR;

	if (elem_ptr == NULL)
	{
		return return_code;
	}

	printf("\tstruct Bin_tree_elem elem_ptr [%p]\n", elem_ptr);
	printf("\t{\n");

	printf("\t\telem = \"" BIN_TREE_ELEM_PRINTF_SPEC "\"\n", elem_ptr->elem);
	printf("\t\tleft_child_ptr  = [%p]\n", elem_ptr->left_child_ptr);
	printf("\t\tright_child_ptr = [%p]\n", elem_ptr->right_child_ptr);
	printf("\t\tid = %zu\n", elem_ptr->id);

	printf("\t}\n");

	if ((elem_ptr->left_child_ptr) != NULL)
	{
		print_bin_tree_elem(elem_ptr->left_child_ptr);
	}
	if ((elem_ptr->right_child_ptr) != NULL)
	{
		print_bin_tree_elem(elem_ptr->right_child_ptr);
	}

	return return_code;
}


err_t bin_tree_dump(const struct Bin_tree* const bin_tree_ptr)
{
	CHECK_BIN_TREE()

	printf("struct Bin_tree bin_tree_ptr [%p]\n", bin_tree_ptr);
	printf("{\n");

	printf("\troot_ptr = [%p]\n", bin_tree_ptr->root_ptr);

	print_bin_tree_elem(bin_tree_ptr->root_ptr);

	printf("}\n");

	return return_code;
}


err_t del_bin_tree_elem(struct Bin_tree_elem* elem_ptr)
{
	if ((elem_ptr->left_child_ptr) != NULL)
	{
		del_bin_tree_elem(elem_ptr->left_child_ptr);
	}

	if ((elem_ptr->right_child_ptr) != NULL)
	{
		del_bin_tree_elem(elem_ptr->right_child_ptr);
	}

	printf("before free [%p]\n", elem_ptr);
	if (elem_ptr != NULL)
	{
		free(elem_ptr);	
	}
	
	printf("free [%p]\n", elem_ptr);
	return NO_ERROR;
}


static err_t write_bin_tree_elem_to_file(FILE* const file, const struct Bin_tree_elem* const elem_ptr)
{
	err_t return_code = NO_ERROR;

	if (elem_ptr == NULL)
	{
		fprintf(file, "nil\n");
		return return_code;
	}

	fprintf(file, "{\n");
	fprintf(file, "%s\n", elem_ptr->elem);
	
	write_bin_tree_elem_to_file(file, elem_ptr->left_child_ptr);
	write_bin_tree_elem_to_file(file, elem_ptr->right_child_ptr);

	fprintf(file, "}\n");

	return return_code;
}


err_t write_bin_tree_to_file(const char* const filename, const struct Bin_tree* const bin_tree_ptr)
{
	CHECK_BIN_TREE()

	FILE* file = open_file(filename, "w");

	write_bin_tree_elem_to_file(file, bin_tree_ptr->root_ptr);

	fclose(file);

	return return_code;
}


err_t read_bin_tree_elem_from_file(FILE* const file, struct Bin_tree* const bin_tree_ptr, ssize_t* const position_ptr, struct Bin_tree_elem* const parent_ptr, const bool left_or_right, char* const* const bin_tree_text)
{
	err_t return_code = NO_ERROR;

	printf("%s\n", bin_tree_text[*position_ptr]);

	if (!strncmp(bin_tree_text[*position_ptr], "nil", sizeof("nil")))
	{
		(*position_ptr)++;
		return return_code;
	}

	(*position_ptr)++;

	printf("%s\n", bin_tree_text[*position_ptr]);

	struct Bin_tree_elem* current_elem_ptr =  init_bin_tree_elem(bin_tree_ptr, parent_ptr, left_or_right, bin_tree_text[*position_ptr]);

	(*position_ptr)++;

	printf("%s\n", bin_tree_text[*position_ptr]);

	read_bin_tree_elem_from_file(file, bin_tree_ptr, position_ptr, current_elem_ptr, LEFT_CHILD, bin_tree_text);
	read_bin_tree_elem_from_file(file, bin_tree_ptr, position_ptr, current_elem_ptr, RIGHT_CHILD, bin_tree_text);

	(*position_ptr)++;

	printf("%s\n", bin_tree_text[*position_ptr]);

	return return_code;
}


err_t read_bin_tree_from_file(const char* const filename, struct Bin_tree* const bin_tree_ptr)
{
	err_t return_code = NO_ERROR;

	FILE* file = open_file(filename, "r");

	ssize_t n_strings = 0;

	char** bin_tree_text =  init_text(filename, &n_strings);

	init_bin_tree(bin_tree_ptr, bin_tree_text[1]);

	printf("%s\n", bin_tree_text[1]);

	ssize_t position = 2;
	struct Bin_tree_elem* current_elem_ptr = bin_tree_ptr->root_ptr;

	read_bin_tree_elem_from_file(file, bin_tree_ptr, &position, current_elem_ptr, LEFT_CHILD, bin_tree_text);
	read_bin_tree_elem_from_file(file, bin_tree_ptr, &position, current_elem_ptr, RIGHT_CHILD, bin_tree_text);

	fclose(file);

	return return_code;
}


err_t bin_tree_dtor(struct Bin_tree* bin_tree_ptr)
{
	err_t return_code = bin_tree_verificator(bin_tree_ptr);

	del_bin_tree_elem(bin_tree_ptr->root_ptr);

	return return_code;
}