#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "include/bin_tree.h"
#include "include/graph.h"
#include "include/text.h"


static err_t init_bin_tree_dynamic_ptrs(struct Bin_tree_dynamic_ptrs* ptrs_ptr, const ssize_t start_size)
{
	err_t return_code = NO_ERROR;

	printf("start %s\n", __func__);

	ptrs_ptr->ptrs = (void**) calloc(start_size, sizeof(void*));

	if ((ptrs_ptr->ptrs) == NULL)
	{
		return_code |= UNABLE_TO_CALOC_DYNAMIC_PTRS_ARR;
		return return_code;
	}

	ptrs_ptr->size = NULL;
	ptrs_ptr->capacity = start_size;

	return return_code;
}


static err_t bin_tree_dynamic_ptrs_verificator(const struct Bin_tree_dynamic_ptrs* const ptrs_ptr)
{
	err_t return_code = NO_ERROR;

	if (ptrs_ptr->size < 0) return_code |= INVALID_BIN_TREE_DYNAMIC_PTRS_SIZE;
	if (ptrs_ptr->capacity <= 0) return_code |= INVALID_BIN_TREE_DYNAMIC_PTRS_CAPACITY;
	if ((ptrs_ptr->size) > (ptrs_ptr->capacity)) return_code |= SIZE_BIGGER_THAN_CAPACITY_IN_BIN_TREE_DYNAMIC_PTRS;
	if ((ptrs_ptr->ptrs) == NULL) return_code |= BIN_TREE_DYNAMIC_PTRS_IS_NULL;

	for (ssize_t n_ptr = 0; n_ptr < ptrs_ptr->size; n_ptr++)
	{
		if ((ptrs_ptr->ptrs)[n_ptr] == NULL) return_code |= DYNAMIC_PTR_IS_NULL_IN_BIN_TREE_DYNAMIC_PTRS;
	}
	for (ssize_t n_ptr = ptrs_ptr->size; n_ptr < ptrs_ptr->capacity; n_ptr++)
	{
		if ((ptrs_ptr->ptrs)[n_ptr] != NULL) return_code |= MEMORY_LEAK_IN_BIN_TREE_DYNAMIC_PTRS;
	}

	return return_code;
}


static err_t bin_tree_dynamic_ptrs_free(struct Bin_tree_dynamic_ptrs*  ptrs_ptr)
{
	err_t return_code = bin_tree_dynamic_ptrs_verificator(ptrs_ptr);

	PRINT_IF_ERROR()

	for (ssize_t n_ptr = 0; n_ptr < ptrs_ptr->size; n_ptr++)
	{
		free((ptrs_ptr->ptrs)[n_ptr]);
	}

	free(ptrs_ptr->ptrs);

	ptrs_ptr->ptrs = NULL;
	ptrs_ptr->size = -1;
	ptrs_ptr->capacity = -1;

	return return_code;
}


static err_t bin_tree_dynamic_ptrs_increase(struct Bin_tree_dynamic_ptrs* ptrs_ptr, const ssize_t increase_coef)
{
	err_t return_code = bin_tree_dynamic_ptrs_verificator(ptrs_ptr);

	PRINT_IF_ERROR()

	void** old_ptrs_arr = ptrs_ptr->ptrs;

	ptrs_ptr->ptrs = (void**) realloc(ptrs_ptr->ptrs, ptrs_ptr->capacity * increase_coef * sizeof(void*));

	if (ptrs_ptr->ptrs == NULL)
	{
		return_code |= UNABLE_TO_INCREASE_BIN_TREE_DYNAMIC_PTRS_ARR;
		ptrs_ptr->ptrs = old_ptrs_arr;
	}
	else 
	{
		for (ssize_t n_ptr = ptrs_ptr->capacity; n_ptr < ptrs_ptr->capacity * increase_coef; n_ptr++)
		{
			(ptrs_ptr->ptrs)[n_ptr] = NULL;
		}

		ptrs_ptr->capacity *= increase_coef;
	}

	return return_code;
}


err_t append_new_dynamic_ptr(void* const ptr, struct Bin_tree_dynamic_ptrs* ptrs_ptr)
{
	printf("before verificator\n");

	err_t return_code = bin_tree_dynamic_ptrs_verificator(ptrs_ptr);

	printf("after verificator\n");

	PRINT_IF_ERROR()

	(ptrs_ptr->ptrs)[ptrs_ptr->size] = ptr;
	(ptrs_ptr->size)++;

	if ((ptrs_ptr->size) > (ptrs_ptr->capacity))
	{
		return_code |= bin_tree_dynamic_ptrs_increase(ptrs_ptr, BIN_TREE_DYNAMIC_PTRS_INCREASE_COEF);
	}

	return return_code;
}


struct Bin_tree_elem* init_bin_tree_elem(struct Bin_tree* const bin_tree_ptr, struct Bin_tree_elem* const parent_ptr, const bool left_or_right, const elem_t elem) // left - 0 right - 1
{
	struct Bin_tree_elem* current_elem_ptr = (struct Bin_tree_elem*) calloc(1, sizeof(struct Bin_tree_elem));

	if (parent_ptr != NULL)
	{	
		if (left_or_right == LEFT_CHILD)
		{ 
			parent_ptr->left_child_ptr = current_elem_ptr;
		}
		else 
		{
			parent_ptr->right_child_ptr = current_elem_ptr;
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

	return_code |= init_bin_tree_dynamic_ptrs(&(bin_tree_ptr->ptrs), BIN_TREE_DYNAMIC_PTRS_ARR_START_SIZE);

	printf("before return %s", __func__);

	return return_code;
}


err_t bin_tree_elem_verificator(const struct Bin_tree_elem* const elem_ptr, const struct Bin_tree* const bin_tree_ptr)
{
	err_t return_code = NO_ERROR;

	if (elem_ptr == (elem_ptr->left_child_ptr)) 					return_code |= PARENT_AND_LEFT_CHILD_ARE_SAME;
	if (elem_ptr == (elem_ptr->right_child_ptr)) 					return_code |= PARENT_AND_RIGHT_CHILD_ARE_SAME;
	if (((elem_ptr->right_child_ptr) == (elem_ptr->left_child_ptr)) && ((elem_ptr->left_child_ptr) != NULL)) 	return_code |= LEFT_CHILD_AND_RIGHT_CHILD_ARE_SAME;
	if ((elem_ptr->id) <= 0) 										return_code |= INVALID_BIN_TREE_ELEM_ID;
	if ((elem_ptr->depth_in_tree) <= 0)								return_code |= INVALID_BIN_TREE_ELEM_DEPTH;
	if ((elem_ptr->elem) == NULL) 									return_code |= ELEM_T_IS_NULL;
	if ((elem_ptr->depth_in_tree) > (elem_ptr->id)) 				return_code |= DEPTH_IS_BIGGER_THAN_ID;
	if ((elem_ptr->depth_in_tree) > (bin_tree_ptr->size)) 			return_code |= DEPTH_IS_BIGGER_THAN_BIN_TREE_SIZE;

	if ((elem_ptr->left_child_ptr) != NULL)
	{
		return_code |= bin_tree_elem_verificator(elem_ptr->left_child_ptr, bin_tree_ptr);
	}

	if ((elem_ptr->right_child_ptr) != NULL)
	{
		return_code |= bin_tree_elem_verificator(elem_ptr->right_child_ptr, bin_tree_ptr);
	}

	return return_code;
}


err_t bin_tree_verificator(const struct Bin_tree* const bin_tree_ptr)
{
	err_t return_code = NO_ERROR;

	if ((bin_tree_ptr->root_ptr) == NULL) return_code |= ROOT_PTR_IS_NULL;
	if ((bin_tree_ptr->head_ptr) == NULL) return_code |= HEAD_PTR_IS_NULL;
	if ((bin_tree_ptr->tail_ptr) == NULL) return_code |= TAIL_PTR_IS_NULL;
	if ((bin_tree_ptr->size) <= 0) return_code |= INVALID_BIN_TREE_SIZE;

	return_code |= bin_tree_elem_verificator(bin_tree_ptr->root_ptr, bin_tree_ptr); 

	return_code |= bin_tree_dynamic_ptrs_verificator(&(bin_tree_ptr->ptrs));

	return return_code;
}


err_t print_bin_tree_err(const err_t return_code)
{
	if (return_code & ROOT_PTR_IS_NULL) fprintf(stderr, "root element pointer is NULL\n");
	if (return_code & HEAD_PTR_IS_NULL) fprintf(stderr, "head pointer is NULL\n");
	if (return_code & TAIL_PTR_IS_NULL) fprintf(stderr, "tail pointer is NULL\n");
	if (return_code & INVALID_BIN_TREE_SIZE) fprintf(stderr, "invalid bin tree size\n");

	if (return_code & PARENT_AND_LEFT_CHILD_ARE_SAME) fprintf(stderr, "parent and left child are same. it may cause infinite recursion\n");
	if (return_code & PARENT_AND_RIGHT_CHILD_ARE_SAME) fprintf(stderr, "parent and right child are same. it may cause infinite recursion\n");
	if (return_code & LEFT_CHILD_AND_RIGHT_CHILD_ARE_SAME) fprintf(stderr, "left and right child are same. Tree is not binary.\n");
	if (return_code & INVALID_BIN_TREE_ELEM_ID) fprintf(stderr, "invalid bin tree element id\n");
	if (return_code & INVALID_BIN_TREE_ELEM_DEPTH) fprintf(stderr, "invalid bin tree element depth\n");
	if (return_code & ELEM_T_IS_NULL) fprintf(stderr, "element is null\n");

	if (return_code & DEPTH_IS_BIGGER_THAN_ID) fprintf(stderr, "bin tree element depth is bigger than bin tree element id\n");
	if (return_code & DEPTH_IS_BIGGER_THAN_BIN_TREE_SIZE) fprintf(stderr, "bin tree element depth is bigger than bin tree size\n");

	if (return_code & UNABLE_TO_CALOC_DYNAMIC_PTRS_ARR) fprintf(stderr, "unable to calloc dynamic ptrs arr\n");

	if (return_code & INVALID_BIN_TREE_SIZE) fprintf(stderr, "invalid bin tree dynamic ptrs size\n");
	if (return_code & INVALID_BIN_TREE_DYNAMIC_PTRS_CAPACITY) fprintf(stderr, "invalid bin tree dynamic ptrs capacity\n");

	if (return_code & SIZE_BIGGER_THAN_CAPACITY_IN_BIN_TREE_DYNAMIC_PTRS) fprintf(stderr, "in struct Bin_tree_dynamic_ptrs size is bigger than capacity\n");

	if (return_code & BIN_TREE_DYNAMIC_PTRS_IS_NULL) fprintf(stderr, "pointer on dynamic pointers in binary tree is NULL\n");

	if (return_code & DYNAMIC_PTR_IS_NULL_IN_BIN_TREE_DYNAMIC_PTRS) fprintf(stderr, "dynamic ptr is null in struct Bin_tree_dynamic_ptrs. It will cause segmentation fault when it free\n");

	if (return_code & MEMORY_LEAK_IN_BIN_TREE_DYNAMIC_PTRS) fprintf(stderr, "pointer index in pointer on pointers in struct Bin_tree_dynamic_ptrs is larger, than Bin_tree_dynamic_ptrs size\n");

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

	printf("\tstruct Bin_tree_dynamic_ptrs [%p]\n", &(bin_tree_ptr->ptrs));

	printf("\t{\n");

	for (ssize_t n_ptr = 0; n_ptr < (bin_tree_ptr->ptrs).capacity; n_ptr++)
	{
		printf("\t%zd: [%p]\n", n_ptr, ((bin_tree_ptr->ptrs).ptrs)[n_ptr]); 
	}

	printf("\t}\n");



	print_bin_tree_elem(bin_tree_ptr->root_ptr);

	printf("}\n");

	return return_code;
}


static err_t del_bin_tree_elem(struct Bin_tree_elem* elem_ptr)
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


#ifdef READABLE_PRINT_BIN_TREE_FILE
	#define PRINT_MINI_TABS(depth, outputfile)		\
	for (ssize_t i = 0; i < depth - 1; i++)			\
	{												\
		fputc('\t', outputfile);					\
	}
	
#else /*READABLE_PRINT_BIN_TREE_FILE*/
	#define PRINT_MINI_TABS(depth, outputfile)

#endif /*READABLE_PRINT_BIN_TREE_FILE*/



static err_t write_bin_tree_elem_to_file(FILE* const file, const struct Bin_tree_elem* const elem_ptr, const ssize_t parent_depth)
{
	err_t return_code = NO_ERROR;

	if (elem_ptr == NULL)
	{
		PRINT_MINI_TABS(parent_depth + 1, file);
		
		fprintf(file, "nil\n");
		return return_code;
	}

	PRINT_MINI_TABS(elem_ptr->depth_in_tree, file);
	
	fprintf(file, "{\n");

	PRINT_MINI_TABS(elem_ptr->depth_in_tree, file);	
	fprintf(file, "%s\n", elem_ptr->elem);

	write_bin_tree_elem_to_file(file, elem_ptr->left_child_ptr, elem_ptr->depth_in_tree);
	write_bin_tree_elem_to_file(file, elem_ptr->right_child_ptr, elem_ptr->depth_in_tree);

	PRINT_MINI_TABS(elem_ptr->depth_in_tree, file);
	fprintf(file, "}\n");

	return return_code;
}



err_t write_bin_tree_to_file(const char* const filename, const struct Bin_tree* const bin_tree_ptr)
{
	CHECK_BIN_TREE()

	FILE* file = open_file(filename, "w");

	write_bin_tree_elem_to_file(file, bin_tree_ptr->root_ptr, (bin_tree_ptr->root_ptr)->depth_in_tree);

	fclose(file);

	return return_code;
}

#undef PRINT_MINI_TABS


static err_t read_bin_tree_elem_from_file(struct Bin_tree* const bin_tree_ptr, ssize_t* const position_ptr, struct Bin_tree_elem* const parent_ptr, const bool left_or_right, char* const* const bin_tree_text)
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

	read_bin_tree_elem_from_file(bin_tree_ptr, position_ptr, current_elem_ptr, LEFT_CHILD, bin_tree_text);
	read_bin_tree_elem_from_file(bin_tree_ptr, position_ptr, current_elem_ptr, RIGHT_CHILD, bin_tree_text);

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

	append_new_dynamic_ptr((void*)(bin_tree_text[0]), &(bin_tree_ptr->ptrs));

	append_new_dynamic_ptr((void*)bin_tree_text, &(bin_tree_ptr->ptrs));

	printf("before append_new_dynamic_ptr\n");

	ssize_t position = 2;
	struct Bin_tree_elem* current_elem_ptr = bin_tree_ptr->root_ptr;

	read_bin_tree_elem_from_file(bin_tree_ptr, &position, current_elem_ptr, LEFT_CHILD, bin_tree_text);
	read_bin_tree_elem_from_file(bin_tree_ptr, &position, current_elem_ptr, RIGHT_CHILD, bin_tree_text);

	fclose(file);

	return return_code;
}


err_t bin_tree_dtor(struct Bin_tree* bin_tree_ptr)
{
	err_t return_code = bin_tree_verificator(bin_tree_ptr);

	//return_code |= del_bin_tree_elem(bin_tree_ptr->root_ptr);

	return_code |= bin_tree_dynamic_ptrs_free(&(bin_tree_ptr->ptrs));

	return return_code;
}