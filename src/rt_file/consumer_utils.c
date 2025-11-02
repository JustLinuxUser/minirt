/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consumer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 12:44:16 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 17:17:29 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_parser.h"
#include "rt_consumer.h"
#include "rt_tokenizer.h"
#include <limits.h>

bool	check_range(t_rt_consumer *consumer, t_rt_node nd, float min, float max)
{
	int	i;

	ft_assert(nd.token.t == RT_TUPLE);
	consumer->last_node = nd;
	consumer->range_start = min;
	consumer->range_end = max;
	i = 0;
	while (i < nd.token.tuple_len)
	{
		if (nd.token.vals_f[i] < min || nd.token.vals_f[i] > max)
		{
			consumer->err = RT_ERR_OUT_OF_RANGE;
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	check_unused_dict(t_rt_consumer *consumer, t_vec_rt_kv dict)
{
	size_t	i;

	i = 0;
	while (i < dict.len)
	{
		if (!dict.buff[i].used)
		{
			consumer->err = RT_ERR_KEY_NOT_USED;
			consumer->last_key = dict.buff[i].k;
			return (false);
		}
		if (!check_unused(consumer, dict.buff[i].v))
			return (false);
		i++;
	}
	return (true);
}

bool	check_unused(t_rt_consumer *consumer, t_rt_node nd)
{
	size_t	i;

	if (!nd.used)
	{
		consumer->err = RT_ERR_NODE_NOT_USED;
		consumer->last_node = nd;
		return (false);
	}
	i = 0;
	while (i < nd.list.len)
	{
		if (!check_unused(consumer, nd.list.buff[i]))
			return (false);
		i++;
	}
	if (!check_unused_dict(consumer, nd.dict))
		return (false);
	return (true);
}
