#ifndef AKINATOR_H
	#define AKINATOR_H

	err_t akinator(struct Bin_tree* const bin_tree_ptr);
	err_t find_all_signes(struct Bin_tree* const bin_tree_ptr, const char* const name, ssize_t name_size);
	err_t compare_two_names(struct Bin_tree* const bin_tree_ptr, const char* const name1, ssize_t name1_size, const char* const name2, ssize_t name2_size);

	enum AkinatorExeptions
	{
		BAD_ANSWER = 1 << 0,
	};
	
#endif /* AKINATOR_H */
