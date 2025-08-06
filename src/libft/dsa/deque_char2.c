/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_char2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 00:31:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/02/05 00:23:28 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "deque_char.h"
#include "../utils/utils.h"
#include "../libft.h"

char	deque_char_idx(t_deque_char *ret, int idx)
{
	char	i;

	ft_assert(idx < ret->len);
	i = ret->buff[(ret->start + idx) % ret->cap];
	return (i);
}

int	deque_char_clone(t_deque_char *ret, const t_deque_char proto)
{
	int	i;

	if (deque_char_init(ret, proto.cap))
		return (1);
	i = 0;
	while (i < proto.len)
	{
		deque_char_push_end(ret, proto.buff[(proto.start + i) % proto.cap]);
		i++;
	}
	return (0);
}

void	deque_char_double_if_needed(t_deque_char *ret)
{
	t_deque_char	new;

	if (ret->len < ret->cap)
		return ;
	deque_char_init(&new, ret->cap * 2 + 1);
	while (ret->len)
	{
		deque_char_push_end(&new, deque_char_pop_end(ret));
	}
	free(ret->buff);
	ft_memcpy(ret, &new, sizeof(new));
	return ;
}
