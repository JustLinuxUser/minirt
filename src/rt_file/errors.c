/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 08:41:12 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 08:41:12 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_parser.h"
#include "rt_tokenizer.h"
#include "../libft/ft_printf/ft_printf.h"
#include "errors.h"

void	print_tokenizer_err(t_rt_tokenizer *tokenizer)
{
	t_error	e;

	ft_assert(tokenizer->err);
	e = (t_error){
		.file = &tokenizer->file,
		.start_idx = tokenizer->i,
		.len = 1,
		.warn_level = RT_ERROR,
	};
	print_warn_lvl(e.warn_level);
	if (tokenizer->err == RT_ERR_TUPLE_TOO_LARGE)
		ft_printf("Tuples can't have more then 3 numbers\n");
	else if (tokenizer->err == RT_ERR_INVALID_NUM)
		ft_printf("Failed to parse number:\n");
	else if (tokenizer->err == RT_ERR_UNCLOSED_QUOTE)
		ft_printf("Unclosed quote:\n");
	else if (tokenizer->err == RT_ERR_UNKNOWN_CHAR)
		ft_printf("Unexpected character:\n");
	else
		ft_assert("Should not be here!" == 0);
	print_line_highlight_range_col(e);
}

void	print_pareser_dup_key_error(t_rt_parser *parser)
{
	t_error	e;

	e = (t_error){
		.file = &parser->tokenizer.file,
		.start_idx = parser->tokenizer.curr_tok.start_idx,
		.len = parser->tokenizer.curr_tok.len,
		.warn_level = RT_ERROR,
	};
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
}

void	print_parser_err(t_rt_parser *parser)
{
	t_error	e;

	e = (t_error){
		.file = &parser->tokenizer.file,
		.start_idx = parser->tokenizer.curr_tok.start_idx,
		.len = parser->tokenizer.curr_tok.len,
		.warn_level = RT_ERROR,
	};
	print_warn_lvl(e.warn_level);
	ft_assert(parser->err);
	if (parser->err == RT_ERR_UNEXPECTED_TOKEN)
		ft_printf("Unexpected token:\n");
	else if (parser->err == RT_ERR_NEW_SYNTAX_IN_OLD_CONTEXT)
		ft_printf("Can't use v2 syntax in a v1 list\n");
	else if (parser->err == RT_ERR_DUPLICATE_KEY)
	{
		print_pareser_dup_key_error(parser);
		return ;
	}
	else
		ft_assert("Should not be here!" == 0);
	ft_assert(peek_token_type(&parser->tokenizer) != RT_NONE);
	print_line_highlight_range_col(e);
}
