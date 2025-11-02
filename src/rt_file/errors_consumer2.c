/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_consumer2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 08:46:00 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 08:46:00 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"
#include "rt_consumer.h"
#include "../libft/ft_printf/ft_printf.h"

bool	print_consumer_err_6(t_rt_consumer *consumer)
{
	t_error	e;

	e = (t_error){
		.file = &consumer->parser.tokenizer.file,
		.warn_level = RT_WARN,
	};
	if (consumer->err == RT_ERR_OUT_OF_RANGE)
	{
		ft_assert(consumer->last_node.token.t == RT_TUPLE);
		e.start_idx = consumer->last_node.token.start_idx;
		e.len = consumer->last_node.token.len;
		print_warn_lvl(ANSI_NUM_RED);
		ft_printf(
			ANSI_RESET"The tuple \""ANSI_BOLD ANSI_YELLOW"%.*s"ANSI_RESET
			"\" contains a value outside the range ["ANSI_BOLD ANSI_GREEN"%i"
			ANSI_RESET", "ANSI_BOLD ANSI_GREEN"%i"ANSI_RESET"]\n",
			consumer->last_node.token.len,
			consumer->parser.tokenizer.file.contents.buff
			+ consumer->last_node.token.start_idx,
			(int)consumer->range_start,
			(int)consumer->range_end);
		print_line_highlight_range_col(e);
		return (true);
	}
	return (false);
}

bool	print_consumer_err_7(t_rt_consumer *consumer)
{
	t_error	e;

	e = (t_error){
		.file = &consumer->parser.tokenizer.file,
		.warn_level = RT_ERROR,
	};
	if (consumer->err == RT_ERR_FAILED_PROCESSING_KEY)
	{
		ft_assert(consumer->last_key.t != RT_NONE);
		e.start_idx = consumer->last_key.start_idx;
		e.len = consumer->last_key.len;
		e.warn_level = ANSI_NUM_YELLOW;
		print_warn_lvl(ANSI_NUM_RED);
		ft_printf(
			ANSI_RESET "Got an error while processing the key \""ANSI_BOLD
			ANSI_YELLOW"%.*s"ANSI_RESET"\"\n", consumer->last_key.len,
			consumer->parser.tokenizer.file.contents.buff
			+ consumer->last_key.start_idx);
		print_line_highlight_range_col(e);
		return (true);
	}
	return (false);
}

void	print_consumer_err(t_rt_consumer *consumer)
{
	if (!print_consumer_err_1(consumer)
		&& !print_consumer_err_2(consumer)
		&& !print_consumer_err_3(consumer)
		&& !print_consumer_err_4(consumer)
		&& !print_consumer_err_5(consumer)
		&& !print_consumer_err_6(consumer)
		&& !print_consumer_err_7(consumer)
	)
		ft_assert("Unreachable" == 0);
}
