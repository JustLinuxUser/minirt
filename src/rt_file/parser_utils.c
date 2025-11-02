/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 09:10:34 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 09:10:34 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "rt_parser.h"

bool	is_rvalue(enum e_RT_TT tt)
{
	if (tt == RT_TUPLE
		|| tt == RT_BOOL
		|| tt == RT_STRING
		|| tt == RT_LBRACE
		|| tt == RT_LBRACKET)
		return (true);
	else
		return (false);
}

bool	is_leaf(enum e_RT_TT tt)
{
	if (tt == RT_TUPLE
		|| tt == RT_BOOL
		|| tt == RT_STRING)
		return (true);
	return (false);
}

enum e_RT_NODE_TYPE	tuple_token_type(t_rt_token t)
{
	ft_assert(t.t == RT_TUPLE);
	if (t.is_int)
		return (RT_ND_TUPLE_I1 + t.tuple_len - 1);
	return (RT_ND_TUPLE_F1 + t.tuple_len - 1);
}

bool	unexpected_err(t_rt_parser *parser)
{
	parser->err = RT_ERR_UNEXPECTED_TOKEN;
	return (false);
}

bool	duplicate_key_err(t_rt_parser *parser,
						t_rt_token prev_key,
						t_rt_token curr) {
	parser->err = RT_ERR_DUPLICATE_KEY;
	parser->err_ref_tok = curr;
	parser->err_ref_tok2 = prev_key;
	return (false);
}
