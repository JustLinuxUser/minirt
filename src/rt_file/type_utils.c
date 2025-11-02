/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:38:02 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 19:38:02 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_parser.h"

enum e_RT_NODE_TYPE	effective_type(enum e_RT_NODE_TYPE target,
						enum e_RT_NODE_TYPE got)
{
	if (target >= RT_ND_TUPLE_F1 && target <= RT_ND_TUPLE_F3
		&& got >= RT_ND_TUPLE_I1 && got <= RT_ND_TUPLE_I3)
		return ((RT_ND_TUPLE_F1 - RT_ND_TUPLE_I1) + got);
	return (got);
}

bool	rt_type_cmp(enum e_RT_NODE_TYPE target, enum e_RT_NODE_TYPE got)
{
	if (effective_type(target, got) == target)
		return (true);
	return (false);
}

char	*type_to_str(enum e_RT_NODE_TYPE t)
{
	if (t == RT_ND_NONE)
		ft_assert("Unreachable" == 0);
	if (t == RT_ND_LIST)
		return ("a list");
	if (t == RT_ND_DICT)
		return ("a dictionary");
	if (t == RT_ND_STRING)
		return ("a string");
	if (t == RT_ND_IDENT)
		return ("an identifier");
	if (t == RT_ND_TUPLE_F1)
		return ("a float");
	if (t == RT_ND_TUPLE_F2)
		return ("a tuple of 2 floats");
	if (t == RT_ND_TUPLE_F3)
		return ("a tuple of 3 floats");
	if (t == RT_ND_TUPLE_I1)
		return ("an int");
	if (t == RT_ND_TUPLE_I2)
		return ("a tuple of 2 ints");
	if (t == RT_ND_TUPLE_I3)
		return ("a tuple of 3 ints");
	if (t == RT_ND_BOOL)
		return ("a boolean");
	return (0);
}
