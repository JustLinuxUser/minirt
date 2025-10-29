#pragma once

#include <stdbool.h>
#include "../libft/dsa/dyn_str.h"

enum RT_TT {
	RT_NONE = 0,

	RT_IDENT,
	RT_TUPLE,
	RT_STRING,
	RT_LBRACE,		// {
	RT_RBRACE,		// }
	RT_LBRACKET,	// [
	RT_RBRACKET,	// ]
	RT_COLON,		// :
	RT_EOF,
};

typedef struct s_rt_token {
	enum RT_TT t;
	int start_idx;
	int len;
	// Tuple specific parts (unions are annoying in C)
	int tuple_len;

	// an int can be converted to a float and back losslessly in a range
	// [-16777216, 16777216], and we only store integers from 0..255,
	// so representing both int tuples and float tuples as float tuples is fine
	float vals_f[3];
	bool	is_int;
} t_rt_token;

enum RT_ERR_TYPE {
	RT_ERR_NONE = 0,
	// tokenizer errors
	RT_ERR_TUPLE_TOO_LARGE,
	RT_ERR_INVALID_NUM,
	RT_ERR_UNCLOSED_QUOTE,
	RT_ERR_UNKNOWN_CHAR,

	// Parser errors
	RT_ERR_UNEXPECTED_TOKEN,
	RT_ERR_NEW_SYNTAX_IN_OLD_CONTEXT,
	RT_ERR_DUPLICATE_KEY,

	// Type errors
	RT_ERR_INVALID_TYPE,
	RT_ERR_KEY_NOT_USED,
	RT_ERR_NODE_NOT_USED,
	RT_ERR_ARR_TOO_SHORT,
	RT_ERR_NOT_FOUND_DICT,
	RT_ERR_OUT_OF_RANGE,
};

typedef struct t_file {
	char *fname;

	t_dyn_str	contents;
} t_file;
typedef struct s_rt_tokenizer {
	// unallocated
	t_file		file;
	int 		i;

	enum RT_ERR_TYPE err;

	// Used to record the location of an open quote,
	// or the start of the tuple for better error messages
	int err_ref;

	// a token for peeking
	bool has_token;
	t_rt_token curr_tok;
} t_rt_tokenizer;
enum RT_TT peek_token_type(t_rt_tokenizer *tokenizer);
bool peek_token(t_rt_tokenizer *tokenizer, t_rt_token *ret);
bool consume_token(t_rt_tokenizer *tokenizer, t_rt_token *ret);
t_rt_token consume_token_panic(t_rt_tokenizer *tokenizer);
void print_tokenizer_err(t_rt_tokenizer *tokenizer);
