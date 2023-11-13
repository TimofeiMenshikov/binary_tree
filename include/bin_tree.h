#ifndef BIN_TREE_H
	#define BIN_TREE_H

	#define BIN_TREE_ELEM_PRINTF_SPEC "%s"

	const ssize_t MAX_STR_SIZE = 100;

	typedef unsigned int err_t;
	typedef char* elem_t;

	enum BinTreeErrors
	{
		NO_ERROR = 0
	};

	enum BinTreeExeptions
	{
		NULL_PARENT = 1 << 0
	};

	enum BinTreeElemChildren
	{
		LEFT_CHILD = 0,
		RIGHT_CHILD = 1
	};

	struct  Bin_tree_elem
	{
		struct Bin_tree_elem* left_child_ptr;
		struct Bin_tree_elem* right_child_ptr;

		ssize_t depth_in_tree;
		size_t id;

		struct Bin_tree_elem* next_ptr;
		struct Bin_tree_elem* prev_ptr;

		elem_t elem;
	};

	struct Bin_tree
	{
		struct Bin_tree_elem* root_ptr;
		struct Bin_tree_elem* head_ptr;
		struct Bin_tree_elem* tail_ptr;

		ssize_t size;
	};

	err_t init_bin_tree(struct Bin_tree* const bin_tree_ptr, const elem_t first_elem);
	struct Bin_tree_elem* init_bin_tree_elem(struct Bin_tree* const bin_tree_ptr, struct Bin_tree_elem* const parent_ptr, const bool left_or_right, const elem_t elem);
	err_t bin_tree_verificator(const struct Bin_tree* const bin_tree_ptr);
	err_t print_bin_tree_err(const err_t return_code);
	err_t print_bin_tree_elem(const struct Bin_tree_elem* const elem_ptr);
	err_t bin_tree_dump(const struct Bin_tree* const bin_tree_ptr);

	err_t bin_tree_dtor(struct Bin_tree* bin_tree_ptr);

	err_t write_bin_tree_to_file(const char* const filename, const struct Bin_tree* const bin_tree_ptr);
	err_t read_bin_tree_from_file(const char* const filename, struct Bin_tree* const bin_tree_ptr);

	#define CHECK_BIN_TREE()									\
	err_t return_code = bin_tree_verificator(bin_tree_ptr);		\
	if (return_code != NO_ERROR)								\
	{															\
		print_bin_tree_err(return_code);						\
		return return_code;										\
	}															


#endif /* BIN_TREE_H */
