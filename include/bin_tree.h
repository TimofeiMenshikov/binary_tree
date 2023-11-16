#ifndef BIN_TREE_H
	#define BIN_TREE_H

	#define BIN_TREE_ELEM_PRINTF_SPEC "%s"

	const ssize_t MAX_STR_SIZE = 100;

	typedef unsigned int err_t;
	typedef char* elem_t;

	const ssize_t BIN_TREE_DYNAMIC_PTRS_ARR_START_SIZE = 5;

	const ssize_t BIN_TREE_DYNAMIC_PTRS_INCREASE_COEF = 2;

	enum BinTreeErrors
	{
		NO_ERROR = 0,
		ROOT_PTR_IS_NULL = 1 << 0,
		HEAD_PTR_IS_NULL = 1 << 1,
		TAIL_PTR_IS_NULL = 1 << 2,
		INVALID_BIN_TREE_SIZE = 1 << 3,		
	};

	enum BinTreeElemErrors
	{
		PARENT_AND_LEFT_CHILD_ARE_SAME 		= 1 << 4,
		PARENT_AND_RIGHT_CHILD_ARE_SAME 	= 1 << 5,
		LEFT_CHILD_AND_RIGHT_CHILD_ARE_SAME = 1 << 6,
		INVALID_BIN_TREE_ELEM_ID 			= 1 << 7,
		INVALID_BIN_TREE_ELEM_DEPTH			= 1 << 8,
		ELEM_T_IS_NULL						= 1 << 9,
		DEPTH_IS_BIGGER_THAN_ID				= 1 << 12,
		DEPTH_IS_BIGGER_THAN_BIN_TREE_SIZE	= 1 << 13					
	};


	enum BinTreeDynamicPtrsErrors
	{
		UNABLE_TO_CALOC_DYNAMIC_PTRS_ARR 					= 1 << 14,
		INVALID_BIN_TREE_DYNAMIC_PTRS_SIZE 					= 1 << 15,
		INVALID_BIN_TREE_DYNAMIC_PTRS_CAPACITY 				= 1 << 16,
		SIZE_BIGGER_THAN_CAPACITY_IN_BIN_TREE_DYNAMIC_PTRS 	= 1 << 17,
		BIN_TREE_DYNAMIC_PTRS_IS_NULL						= 1 << 18,
		DYNAMIC_PTR_IS_NULL_IN_BIN_TREE_DYNAMIC_PTRS		= 1 << 19,
		MEMORY_LEAK_IN_BIN_TREE_DYNAMIC_PTRS				= 1 << 20,
		UNABLE_TO_INCREASE_BIN_TREE_DYNAMIC_PTRS_ARR		= 1 << 21
	};


	enum BinTreeElemChildren
	{
		LEFT_CHILD = 0,
		RIGHT_CHILD = 1
	};

	struct Bin_tree_dynamic_ptrs
	{
		ssize_t size;
		ssize_t capacity;
		void** ptrs;
	};

	struct  Bin_tree_elem
	{
		struct Bin_tree_elem* left_child_ptr;
		struct Bin_tree_elem* right_child_ptr;

		size_t depth_in_tree;
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

		struct Bin_tree_dynamic_ptrs ptrs;

		size_t size;
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

	err_t bin_tree_elem_verificator(const struct Bin_tree_elem* const elem_ptr, const struct Bin_tree* const bin_tree_ptr);

	err_t append_new_dynamic_ptr(void* const ptr, struct Bin_tree_dynamic_ptrs* ptrs_ptr);

	#define PRINT_IF_ERROR()														\
	if (return_code != NO_ERROR)													\
	{																				\
		printf("error! file: %s func: %s line: %d\n", __FILE__, __func__, __LINE__);\
		print_bin_tree_err(return_code);											\
		return return_code;															\
	}																				\


	#define CHECK_BIN_TREE()								\
	err_t return_code = bin_tree_verificator(bin_tree_ptr);	\
	PRINT_IF_ERROR()										\



#endif /* BIN_TREE_H */
