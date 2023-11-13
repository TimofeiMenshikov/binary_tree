#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "include/bin_tree.h"
#include "include/graph.h"
#include "include/text.h"
#include "include/akinator.h"

#define START_TEST()      																\
	err_t return_code = NO_ERROR;														\
																						\
	printf("___________________________________________________________________\n");	\
																						\
	printf("start %s\n", __func__);														\
																						\
	struct Bin_tree bin_tree;															\
																						\
																						\
																						\
	FILE* html_file = open_file(html_filepath, "w");									\
	assert(html_file);																	\
																						\
	char graph_filepath[MAX_FILENAME_SIZE] = {};										\
																						\
	ssize_t number = 0;																	\


#define END_TEST()																			\
	bin_tree_dtor(&bin_tree);																\
    fclose(html_file);																		\
	RUN_HTML_FILE(graph_folder, html_filename);												\


int main()
{
	START_TEST()

	/*bin_tree_dump(&bin_tree);

	CREATE_ONE_GRAPH()

	char left_name[MAX_STR_SIZE] = "left child";

	init_bin_tree_elem(&bin_tree, bin_tree.root_ptr, LEFT_CHILD, left_name);

	CREATE_ONE_GRAPH()

	char right_name[MAX_STR_SIZE] = "right child";

	init_bin_tree_elem(&bin_tree, bin_tree.root_ptr, RIGHT_CHILD, right_name);

	bin_tree_dump(&bin_tree);

	CREATE_ONE_GRAPH()

	struct Bin_tree_elem* first_left_child_ptr = (bin_tree.root_ptr)->left_child_ptr;

	char left_left_name[MAX_STR_SIZE] = "left left child";

	init_bin_tree_elem(&bin_tree, first_left_child_ptr, LEFT_CHILD, left_left_name);

	CREATE_ONE_GRAPH()

	bin_tree_dump(&bin_tree);

	write_bin_tree_to_file("txt/bin_tree.txt", &bin_tree);*/

	read_bin_tree_from_file("txt/bin_tree.txt", &bin_tree);

	bin_tree_dump(&bin_tree);

	CREATE_ONE_GRAPH()

	akinator(&bin_tree);

	write_bin_tree_to_file("txt/bin_tree.txt", &bin_tree);

	bin_tree_dump(&bin_tree);

	find_all_signes(&bin_tree, "bebra", sizeof("bebra"));

	compare_two_names(&bin_tree, "bebra", sizeof("bebra"), "right left child", sizeof("right left child"));

	CREATE_ONE_GRAPH()

	END_TEST()
}