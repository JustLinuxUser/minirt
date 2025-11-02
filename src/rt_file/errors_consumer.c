/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_consumer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 08:43:14 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 08:43:14 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "rt_types.h"
#include "rt_consumer.h"
#include "../libft/ft_printf/ft_printf.h"

bool	print_consumer_err_1(t_rt_consumer *consumer)
{
	t_error	e;

	e = (t_error){
		.file = &consumer->parser.tokenizer.file,
		.warn_level = RT_ERROR,
	};
	if (consumer->err == RT_ERR_INVALID_TYPE)
	{
		ft_assert(consumer->last_node.t != RT_ND_NONE);
		print_warn_lvl(e.warn_level);
		ft_printf(ANSI_RESET "Expected ""\""ANSI_GREEN ANSI_BOLD "%s" ANSI_RESET
			"\", but got \"" ANSI_BOLD ANSI_RED "%s" ANSI_RESET "\"\n",
			type_to_str(consumer->expected),
			type_to_str(effective_type(consumer->expected,
					consumer->last_node.t)));
		e.start_idx = consumer->last_node.token.start_idx;
		e.len = consumer->last_node.token.len;
		print_line_highlight_range_col(e);
		return (true);
	}
	return (false);
}

bool	print_consumer_err_2(t_rt_consumer *consumer)
{
	t_error	e;

	e = (t_error){
		.file = &consumer->parser.tokenizer.file,
		.warn_level = RT_ERROR,
	};
	if (consumer->err == RT_ERR_KEY_NOT_USED)
	{
		ft_assert(consumer->last_key.t != RT_NONE);
		print_warn_lvl(e.warn_level);
		ft_printf(
			ANSI_RESET "Unknown key: \""ANSI_BOLD ANSI_RED"%.*s\033" ANSI_RESET
			"\"\n", consumer->last_key.len,
			consumer->parser.tokenizer.file.contents.buff
			+ consumer->last_key.start_idx);
		e.start_idx = consumer->last_key.start_idx;
		e.len = consumer->last_key.len;
		print_line_highlight_range_col(e);
		return (true);
	}
	return (false);
}

bool	print_consumer_err_3(t_rt_consumer *consumer)
{
	t_error	e;

	e = (t_error){
		.file = &consumer->parser.tokenizer.file,
		.warn_level = RT_ERROR,
	};
	if (consumer->err == RT_ERR_NODE_NOT_USED)
	{
		ft_assert(consumer->last_node.t != RT_ND_NONE);
		pprint_node(consumer->parser, consumer->last_node, 0);
		e.start_idx = consumer->last_node.token.start_idx;
		e.len = consumer->last_node.token.len;
		print_warn_lvl(ANSI_NUM_RED);
		ft_printf(
			ANSI_RESET"Unused node: \"" ANSI_BOLD ANSI_RED "%.*s"
			ANSI_RESET"\"\n", consumer->last_node.token.len,
			consumer->parser.tokenizer.file.contents.buff
			+ consumer->last_node.token.start_idx);
		print_line_highlight_range_col(e);
		return (true);
	}
	return (false);
}

bool	print_consumer_err_4(t_rt_consumer *consumer)
{
	t_error	e;

	e = (t_error){
		.file = &consumer->parser.tokenizer.file,
		.warn_level = RT_ERROR,
	};
	if (consumer->err == RT_ERR_ARR_TOO_SHORT)
	{
		ft_assert(consumer->last_node.t != RT_ND_NONE);
		e.start_idx = consumer->last_key.start_idx;
		e.len = consumer->last_key.len;
		e.warn_level = ANSI_NUM_YELLOW;
		print_warn_lvl(ANSI_NUM_RED);
		ft_printf(
			ANSI_RESET "List \"" ANSI_BOLD ANSI_YELLOW "%.*s"ANSI_RESET
			"\" too short, expected an item of type: \"" ANSI_BOLD
			ANSI_YELLOW "%s"ANSI_RESET"\" at position "ANSI_BOLD
			ANSI_GREEN"%i\n" ANSI_RESET, consumer->last_key.len,
			consumer->parser.tokenizer.file.contents.buff
			+ consumer->last_key.start_idx,
			type_to_str(consumer->expected), consumer->curr_idx);
		print_line_highlight_range_col(e);
		return (true);
	}
	return (false);
}

bool	print_consumer_err_5(t_rt_consumer *consumer)
{
	t_error	e;

	e = (t_error){
		.file = &consumer->parser.tokenizer.file,
		.warn_level = RT_ERROR,
	};
	if (consumer->err == RT_ERR_NOT_FOUND_DICT)
	{
		ft_assert(consumer->last_node.t != RT_ND_NONE);
		e.start_idx = consumer->last_key.start_idx;
		e.len = consumer->last_key.len;
		e.warn_level = ANSI_NUM_YELLOW;
		print_warn_lvl(ANSI_NUM_RED);
		ft_printf(
			ANSI_RESET "The key \""ANSI_BOLD ANSI_YELLOW"%.*s"ANSI_RESET
			"\" does not contain an identifier \""ANSI_BOLD ANSI_GREEN
			"%s"ANSI_RESET"\" of type: \""ANSI_BOLD ANSI_YELLOW
			"%s"ANSI_RESET"\"\n", consumer->last_key.len,
			consumer->parser.tokenizer.file.contents.buff
			+ consumer->last_key.start_idx, consumer->last_ident,
			type_to_str(consumer->expected));
		print_line_highlight_range_col(e);
		return (true);
	}
	return (false);
}
