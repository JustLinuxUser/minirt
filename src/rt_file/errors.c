#include "rt_parser.h"
#include "rt_consumer.h"
#include "rt_tokenizer.h"
#include "rt_types.h"
#include "../libft/libft.h"

void print_tokenizer_err(t_rt_tokenizer *tokenizer)
{
	ft_assert(tokenizer->err);

	switch (tokenizer->err) {
        case RT_ERR_TUPLE_TOO_LARGE:
			ft_printf("The tuples can't have more then 3 numbers\n");
			break;
        case RT_ERR_INVALID_NUM:
			ft_printf("Failed to parse the number\n");
			break;
        case RT_ERR_UNCLOSED_QUOTE:
			ft_printf("Unclosed quote\n");
			break;
        case RT_ERR_UNKNOWN_CHAR:
			printf("Got an unknown character\n");
			break;
		default:
			ft_assert("Should not be here!" == 0);
        }
		print_line_highlight_range_col(tokenizer->file.buff, tokenizer->i, 1, 31);
}

void print_parser_err(t_rt_parser *parser)
{
	ft_assert(parser->err);

	switch (parser->err) {
		case RT_ERR_UNEXPECTED_TOKEN:
			printf("Got an unexpected token\n");
			break;
		case RT_ERR_NEW_SYNTAX_IN_OLD_CONTEXT:
			printf("Got an unexpected token\n");
			break;
		case RT_ERR_DUPLICATE_KEY:
			printf("Got a duplicate key:\n");
			ft_assert(peek_token_type(&parser->tokenizer) != RT_NONE);
			print_line_highlight_range_col(parser->tokenizer.file.buff,
							  parser->err_ref_tok.start_idx, parser->err_ref_tok.len, 31);
			printf("Overwriting the previous definition of this key:\n");
			ft_assert(parser->err_ref_tok.t != RT_NONE);
			print_line_highlight_range_col(parser->tokenizer.file.buff,
							  parser->err_ref_tok2.start_idx, parser->err_ref_tok2.len, 32);
			return;
		default: 
			ft_assert("Should not be here!" == 0);
	}
	ft_assert(peek_token_type(&parser->tokenizer) != RT_NONE);
	print_line_highlight_range_col(parser->tokenizer.file.buff,
		parser->tokenizer.curr_tok.start_idx, parser->tokenizer.curr_tok.len, 31);
}

void print_consumer_err(t_rt_consumer* consumer)
{
	switch (consumer->err) {
        case RT_ERR_INVALID_TYPE:
			ft_assert(consumer->last_node.t != RT_ND_NONE);
			printf(
				"Expected \033[1m\033[32m%s\033[0m, but got \033[1m\033[31m%s\033[0m\n", type_to_str(consumer->expected),
				type_to_str(effective_type(consumer->expected, consumer->last_node.t)));
			print_line_highlight_range_col(consumer->parser.tokenizer.file.buff,
								  consumer->last_node.token.start_idx,
								  consumer->last_node.token.len, 31);
			break;
        case RT_ERR_KEY_NOT_USED:
			ft_assert(consumer->last_key.t != RT_NONE);
			printf(
				"Unknown key: \033[1m\033[31m%.*s\033[0m\n",
				consumer->last_key.len,
				consumer->parser.tokenizer.file.buff + consumer->last_key.start_idx);
			print_line_highlight_range_col(consumer->parser.tokenizer.file.buff,
								  consumer->last_key.start_idx,
								  consumer->last_key.len, 31);
			break;

        case RT_ERR_NODE_NOT_USED:
			ft_assert(consumer->last_node.t != RT_ND_NONE);
			pprint_node(consumer->parser, consumer->last_node, 0);
			printf(
				"Unused node: \033[1m\033[31m%.*s\033[0m\n",
				consumer->last_node.token.len,
				consumer->parser.tokenizer.file.buff + consumer->last_node.token.start_idx);
			print_line_highlight_range_col(consumer->parser.tokenizer.file.buff,
								  consumer->last_node.token.start_idx,
								  consumer->last_node.token.len, 31);
			break;

        case RT_ERR_NOT_FOUND_ARR:
			ft_assert(consumer->last_node.t != RT_ND_NONE);
			printf(
				"List \033[1m\033[33m%.*s\033[0m too short, expected item of type: \"\033[1m\033[33m%s\033[0m\" at position \033[1m\033[32m%i\033[0m\n",
				consumer->last_key.len,
				consumer->parser.tokenizer.file.buff + consumer->last_key.start_idx,
				type_to_str(consumer->expected), consumer->last_idx);
			print_line_highlight_range_col(consumer->parser.tokenizer.file.buff,
								  consumer->last_key.start_idx,
								  consumer->last_key.len, 33);
			break;

        case RT_ERR_NOT_FOUND_DICT:
			ft_assert(consumer->last_node.t != RT_ND_NONE);
			printf(
				"The key \033[1m\033[33m%.*s\033[0m does not contain an identifier \033[1m\033[32m%s\033[0m of type: \"\033[1m\033[33m%s\033[0m\"\n",
				consumer->last_key.len,
				consumer->parser.tokenizer.file.buff + consumer->last_key.start_idx,
				consumer->last_ident,
				type_to_str(consumer->expected));
			print_line_highlight_range_col(consumer->parser.tokenizer.file.buff,
								  consumer->last_key.start_idx,
								  consumer->last_key.len, 33);
			break;

		case RT_ERR_OUT_OF_RANGE:
			ft_assert(consumer->last_node.token.t == RT_TUPLE);
			printf(
				"The tuple \033[1m\033[31m%.*s\033[0m contains value outside the range [\033[1m\033[32m%f\033[0m, \033[1m\033[32m%f\033[0m]\n",
				consumer->last_node.token.len,
				consumer->parser.tokenizer.file.buff + consumer->last_node.token.start_idx,
				consumer->range_start,
				consumer->range_end);
			print_line_highlight_range_col(consumer->parser.tokenizer.file.buff,
								  consumer->last_node.token.start_idx,
								  consumer->last_node.token.len, 31);
			break;
		default: 
			ft_assert("Unreachable" == 0);
	}

}

