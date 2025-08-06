/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_int2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 00:31:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/04/18 21:19:08 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "deque_int.h"
#include "../utils/utils.h"
#include <stdbool.h>

int	deque_int_idx(t_deque_int *ret, int idx)
{
	int	i;

	ft_assert(idx < ret->len);
	i = ret->buff[(ret->start + idx) % ret->cap];
	return (i);
}

int	deque_int_clone(t_deque_int *ret, const t_deque_int proto)
{
	int	i;

	if (deque_int_init(ret, proto.cap))
		return (1);
	i = 0;
	while (i < proto.len)
	{
		deque_int_push_end(ret, proto.buff[(proto.start + i) % proto.cap]);
		i++;
	}
	return (0);
}
