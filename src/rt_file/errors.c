#include "rt_parser.h"
#include "rt_consumer.h"
#include "rt_tokenizer.h"
#include "rt_types.h"
#include "../libft/libft.h"
#include <stdio.h>
#include "errors.h"

void print_tokenizer_err(t_rt_tokenizer *tokenizer)
{
	t_error	e;

	ft_assert(tokenizer->err);
	e = (t_error) {
		.file = &tokenizer->file,
		.start_idx = tokenizer->i,
		.len = 1,
		.warn_level = RT_ERROR,
	};
	print_warn_lvl(e.warn_level);
	switch (tokenizer->err) {
        case RT_ERR_TUPLE_TOO_LARGE:
			ft_printf("Tuples can't have more then 3 numbers\n");
			break;
        case RT_ERR_INVALID_NUM:
			ft_printf("Failed to parse number:\n");
			break;
        case RT_ERR_UNCLOSED_QUOTE:
			ft_printf("Unclosed quote:\n");
			break;
        case RT_ERR_UNKNOWN_CHAR:
			ft_printf("Unexpected character:\n");
			break;
		default:
			ft_assert("Should not be here!" == 0);
        }
		print_line_highlight_range_col(e);
}

void print_parser_err(t_rt_parser *parser)
{
	ft_assert(parser->err);

	t_error	e;

	e = (t_error) {
		.file = &parser->tokenizer.file,
		.start_idx = parser->tokenizer.curr_tok.start_idx,
		.len = parser->tokenizer.curr_tok.len,
		.warn_level = RT_ERROR,
	};
	print_warn_lvl(e.warn_level);

	switch (parser->err) {
		case RT_ERR_UNEXPECTED_TOKEN:
			ft_printf("Unexpected token:\n");
			break;
		case RT_ERR_NEW_SYNTAX_IN_OLD_CONTEXT:
			ft_printf("Can't use v2 syntax in a v1 list\n");
			break;
		case RT_ERR_DUPLICATE_KEY:
			ft_assert(parser->err_ref_tok.t != RT_NONE);
			ft_assert(parser->err_ref_tok2.t != RT_NONE);

			ft_printf("Got a duplicate key:\n");

			e.start_idx = parser->err_ref_tok.start_idx;
			e.len = parser->err_ref_tok.len;
			print_line_highlight_range_col(e);

			e.start_idx = parser->err_ref_tok2.start_idx;
			e.len = parser->err_ref_tok2.len;
			e.warn_level = RT_HELP;
			print_warn_lvl(e.warn_level);

			ft_printf("Overwriting the previous definition of this key:\n");
			print_line_highlight_range_col(e);
			return;
		default: 
			ft_assert("Should not be here!" == 0);
	}
	ft_assert(peek_token_type(&parser->tokenizer) != RT_NONE);
	print_line_highlight_range_col(e);
}

void print_consumer_err(t_rt_consumer* consumer)
{
	t_error	e;

	e = (t_error) {
		.file = &consumer->parser.tokenizer.file,
		.warn_level = RT_ERROR,
	};
	switch (consumer->err) {
        case RT_ERR_INVALID_TYPE:
			ft_assert(consumer->last_node.t != RT_ND_NONE);
			print_warn_lvl(e.warn_level);
			ft_printf(
				ANSI_RESET "Expected ""\""ANSI_GREEN ANSI_BOLD "%s" ANSI_RESET "\", but got \"" ANSI_BOLD ANSI_RED "%s" ANSI_RESET "\"\n", type_to_str(consumer->expected),
				type_to_str(effective_type(consumer->expected, consumer->last_node.t)));
			e.start_idx = consumer->last_node.token.start_idx;
			e.len = consumer->last_node.token.len;
			print_line_highlight_range_col(e);
			break;
        case RT_ERR_KEY_NOT_USED:
			ft_assert(consumer->last_key.t != RT_NONE);
			print_warn_lvl(e.warn_level);
			ft_printf(
				ANSI_RESET "Unknown key: \""ANSI_BOLD ANSI_RED"%.*s\033" ANSI_RESET"\"\n",
				consumer->last_key.len,
				consumer->parser.tokenizer.file.contents.buff + consumer->last_key.start_idx);

			e.start_idx = consumer->last_key.start_idx;
			e.len = consumer->last_key.len;
			print_line_highlight_range_col(e);
			break;

        case RT_ERR_NODE_NOT_USED:
			ft_assert(consumer->last_node.t != RT_ND_NONE);
			pprint_node(consumer->parser, consumer->last_node, 0);

			e.start_idx = consumer->last_node.token.start_idx;
			e.len = consumer->last_node.token.len;

			print_warn_lvl(ANSI_NUM_RED);
			ft_printf(
				"Unused node: " ANSI_BOLD ANSI_RED "%.*s\n" ANSI_RESET,
				consumer->last_node.token.len,
				consumer->parser.tokenizer.file.contents.buff + consumer->last_node.token.start_idx);
			print_line_highlight_range_col(e);
			break;

        case RT_ERR_ARR_TOO_SHORT:
			ft_assert(consumer->last_node.t != RT_ND_NONE);

			e.start_idx = consumer->last_key.start_idx;
			e.len = consumer->last_key.len;
			e.warn_level = ANSI_NUM_YELLOW;

			print_warn_lvl(ANSI_NUM_RED);
			ft_printf(
				ANSI_RESET "List \"" ANSI_BOLD ANSI_YELLOW "%.*s"ANSI_RESET"\" too short, expected an item of type: \"" ANSI_BOLD  ANSI_YELLOW "%s"ANSI_RESET"\" at position "ANSI_BOLD  ANSI_GREEN"%i\n" ANSI_RESET,
				consumer->last_key.len,
				consumer->parser.tokenizer.file.contents.buff + consumer->last_key.start_idx,
				type_to_str(consumer->expected), consumer->curr_idx);

			print_line_highlight_range_col(e);
			break;

        case RT_ERR_NOT_FOUND_DICT:
			ft_assert(consumer->last_node.t != RT_ND_NONE);

			e.start_idx = consumer->last_key.start_idx;
			e.len = consumer->last_key.len;
			e.warn_level = ANSI_NUM_YELLOW;

			print_warn_lvl(ANSI_NUM_RED);
			ft_printf(
				ANSI_RESET "The key \""ANSI_BOLD  ANSI_YELLOW"%.*s"ANSI_RESET"\" does not contain an identifier \""ANSI_BOLD  ANSI_GREEN "%s"ANSI_RESET"\" of type: \""ANSI_BOLD  ANSI_YELLOW"%s"ANSI_RESET"\"\n",
				consumer->last_key.len,
				consumer->parser.tokenizer.file.contents.buff + consumer->last_key.start_idx,
				consumer->last_ident,
				type_to_str(consumer->expected));
			print_line_highlight_range_col(e);
			break;

		case RT_ERR_OUT_OF_RANGE:
			ft_assert(consumer->last_node.token.t == RT_TUPLE);

			e.start_idx = consumer->last_node.token.start_idx;
			e.len = consumer->last_node.token.len;
			e.warn_level = ANSI_NUM_YELLOW;

			print_warn_lvl(ANSI_NUM_RED);

			ft_printf(
				ANSI_RESET"The tuple \""ANSI_BOLD  ANSI_YELLOW"%.*s"ANSI_RESET"\" contains value outside the range ["ANSI_BOLD ANSI_GREEN"%i"ANSI_RESET", "ANSI_BOLD ANSI_GREEN"%i"ANSI_RESET"]\n",
				consumer->last_node.token.len,
				consumer->parser.tokenizer.file.contents.buff + consumer->last_node.token.start_idx,
				(int)consumer->range_start,
				(int)consumer->range_end);
			print_line_highlight_range_col(e);
			break;
		case RT_ERR_FAILED_PROCESSING_KEY:
			ft_assert(consumer->last_key.t != RT_NONE);

			e.start_idx = consumer->last_key.start_idx;
			e.len = consumer->last_key.len;
			e.warn_level = ANSI_NUM_YELLOW;

			print_warn_lvl(ANSI_NUM_RED);
			ft_printf(
				ANSI_RESET "Got an error while processing the key \""ANSI_BOLD  ANSI_YELLOW"%.*s"ANSI_RESET"\"\n",
				consumer->last_key.len,
				consumer->parser.tokenizer.file.contents.buff + consumer->last_key.start_idx);
			print_line_highlight_range_col(e);
			break;
		default: 
			ft_assert("Unreachable" == 0);
	}

}

