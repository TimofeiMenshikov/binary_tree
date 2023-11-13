#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include "include/bin_tree.h"
#include "include/graph.h"
#include "include/text.h"


#define CREATE_DOT_LINE(command_string) fprintf(dot_file, "" command_string "\n");
#define CREATE_HTML_LINE(command_string) fprintf(html_file, "" command_string "\n");

#define RUN_DOT_FILE(graph_filename, dot_filename, graph_file_extension)  								\
{																										\
	char system_command[MAX_COMMANDNAME_SIZE] = {};														\
	sprintf(system_command, "dot -T%s %s -o %s", graph_file_extension, dot_filename, graph_filename);	\
	system(system_command);																				\
}																										\


#define RUN_HTML_FILE(graph_folder, html_filename)												\
{																								\
	char system_command[MAX_COMMANDNAME_SIZE] = {};												\
	sprintf(system_command, "cd %s && %s", graph_folder, html_filename);						\
	printf("%s", system_command);																\
	system(system_command);																		\
}																								\


FILE* open_file(const char* const filename, const char* const modificator)
{
	FILE* inputfile = fopen(filename, modificator);
	assert(inputfile);
	return inputfile;
}


err_t create_one_graph(const struct Bin_tree* const bin_tree_ptr, ssize_t* graph_number_ptr, char *  graph_filepath, const char * const dot_filepath, const char* const graph_file_extension, FILE* html_file)	
{	
	CHECK_BIN_TREE();

	sprintf(graph_filepath, "graph/graph%zd.png", *graph_number_ptr);						
																						
	write_to_dot_file(bin_tree_ptr, dot_filepath);											
																						
																						
																						
	RUN_DOT_FILE(graph_filepath, dot_filepath, graph_file_extension)					
																		
	CREATE_HTML_LINE("<section>")				
	fprintf(html_file, "<img src=graph%zd.png alt=\"struct List list [%p]\"/>\n", *graph_number_ptr, bin_tree_ptr);	
	CREATE_HTML_LINE("</section>")					
																						
	(*graph_number_ptr)++;	

	return return_code;
}


err_t draw_all_bin_tree_elem_dot(const struct Bin_tree* bin_tree_ptr, FILE* const dot_file)
{
	CHECK_BIN_TREE();

	struct Bin_tree_elem* current_elem_ptr = bin_tree_ptr->head_ptr;

	while (current_elem_ptr != NULL)
	{
		fprintf(dot_file, "\"node%zd\"  [ label = \"<f1> %s  | { id = %zd | depth = %zd} \" shape = \"Mrecord\" color =\"Green\"];\n", current_elem_ptr->id, current_elem_ptr->elem, current_elem_ptr->id, current_elem_ptr->depth_in_tree);
		current_elem_ptr = current_elem_ptr->next_ptr;
	}

	return return_code;
}


err_t link_bin_tree_elem_dot(const struct Bin_tree_elem* elem_ptr, FILE* const dot_file)
{
	if ((elem_ptr->left_child_ptr) != NULL)
	{
		fprintf(dot_file, "\"node%zd\" -> \"node%zd\";\n", elem_ptr->id, (elem_ptr->left_child_ptr)->id);
		link_bin_tree_elem_dot(elem_ptr->left_child_ptr, dot_file);
	}
	if ((elem_ptr->right_child_ptr) != NULL)
	{
		fprintf(dot_file, "\"node%zd\" -> \"node%zd\";\n", elem_ptr->id, (elem_ptr->right_child_ptr)->id);
		link_bin_tree_elem_dot(elem_ptr->right_child_ptr, dot_file);
	}

	return NO_ERROR;
}		


err_t write_to_dot_file(const struct Bin_tree* const bin_tree_ptr, const char* const dot_filepath)
{
	CHECK_BIN_TREE();

	FILE* dot_file = open_file(dot_filepath, "w"); 
	assert(dot_file);

	CREATE_DOT_LINE("digraph G {")
	fprintf(dot_file, "rankdir = \"LR\";\n");
	fprintf(dot_file, "graph [splines=ortho];\n");
	CREATE_DOT_LINE("subgraph cluster0 {")

	fprintf(dot_file, "label = \"struct Bin_tree bin_tree [%p]\";", bin_tree_ptr);

	fprintf(dot_file, "fontname=\"Helvetica,Arial,sans-serif\";\n");
	fprintf(dot_file, "node [fontname=\"Helvetica,Arial,sans-serif\" fontsize = \"16\" ];\n");
	fprintf(dot_file, "edge [fontname=\"Helvetica,Arial,sans-serif\"];\n");




	CREATE_DOT_LINE("subgraph cluster1 {")
	fprintf(dot_file, "rankdir = \"TB\";\n");
	fprintf(dot_file, "label = \"bin_tree variables\"");
	fprintf(dot_file, "size [shape=box3d, label=\"size = %zd\"];\n", bin_tree_ptr->size);
	fprintf(dot_file, "tail_ptr [shape=box3d, label=\"tail pointer = [%p]\"];\n", bin_tree_ptr->tail_ptr);
	fprintf(dot_file, "head_ptr [shape=box3d, label=\"head pointer = [%p]\"];\n", bin_tree_ptr->head_ptr);
	fprintf(dot_file, "root_ptr [shape=box3d, label=\"root pointer = [%p]\"];\n", bin_tree_ptr->root_ptr);

	//fprintf(dot_file, "\"node%zd\" [ label = \"struct List_elem list_elem_ptr [%p] <f1> | { <f0> elem = " LIST_ELEM_PRINTF_SPEC " | <f1> elem_number = %zd | <f2> prev_elem_ptr = [%p] | <f3> elem_ptr = [%p] | <f4> next_elem_ptr = [%p] } \" shape = \"Mrecord\", color=\"Yellow\"];\n",elem_number,  list_elem_ptr,  list_elem_ptr->elem, elem_number, list_elem_ptr->prev_elem_ptr, list_elem_ptr, list_elem_ptr->next_elem_ptr);

	CREATE_DOT_LINE("}")	
	CREATE_DOT_LINE("subgraph cluster2 {")

	fprintf(dot_file, "label = \"bin tree elements\"\n");



	//fprintf(dot_file, "\"node%zd\"  [ label = \"<f1> %s  | { id = %zd | depth = %zd} \" shape = \"Mrecord\" color =\"Green\"];", (bin_tree_ptr->root_ptr)->id, (bin_tree_ptr->root_ptr)->elem, (bin_tree_ptr->root_ptr)->id, (bin_tree_ptr->root_ptr)->depth_in_tree);

	return_code |= draw_all_bin_tree_elem_dot(bin_tree_ptr, dot_file);
	return_code |= link_bin_tree_elem_dot(bin_tree_ptr->root_ptr, dot_file);

	ssize_t elem_number = 0;



	/*do 
	{
		fprintf(dot_file, "\"node%zd\" [ label = \"struct List_elem list_elem_ptr [%p] <f1> | { <f0> elem = " LIST_ELEM_PRINTF_SPEC " | <f1> elem_number = %zd | <f2> prev_elem_ptr = [%p] | <f3> elem_ptr = [%p] | <f4> next_elem_ptr = [%p] } \" shape = \"Mrecord\", color=\"Yellow\"];\n",elem_number,  list_elem_ptr,  list_elem_ptr->elem, elem_number, list_elem_ptr->prev_elem_ptr, list_elem_ptr, list_elem_ptr->next_elem_ptr);




		printf("%zd\n", elem_number);


		elem_number++;

		list_elem_ptr = list_elem_ptr->next_elem_ptr;
	} while (list_elem_ptr != list_ptr->zero_elem_ptr);*/



	CREATE_DOT_LINE("} } }")

	fclose(dot_file);


	return return_code;
}