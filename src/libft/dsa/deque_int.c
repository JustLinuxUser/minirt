/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_int.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 00:31:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/01/27 05:11:36 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "deque_int.h"
#include "../utils/utils.h"

int	deque_int_init(t_deque_int *ret, int size)
{
	*ret = (t_deque_int){0};
	ret->buff = malloc(size * sizeof(int));
	if (ret->buff == 0)
		return (1);
	ret->cap = size;
	ret->end = ret->cap - 1;
	return (0);
}

void	deque_int_push_end(t_deque_int *ret, int item)
{
	ft_assert(ret->cap > ret->len);
	ret->end = (ret->end + 1) % ret->cap;
	ret->buff[ret->end] = item;
	ret->len++;
}

void	deque_int_push_start(t_deque_int *ret, int item)
{
	ft_assert(ret->cap > ret->len);
	ret->start = ft_smod(ret->start - 1, ret->cap);
	ret->buff[ret->start] = item;
	ret->len++;
}

int	deque_int_pop_end(t_deque_int *ret)
{
	int	i;

	ft_assert(ret->len > 0);
	i = ret->buff[ret->end];
	ret->end = ft_smod(ret->end - 1, ret->cap);
	ret->len--;
	return (i);
}

int	deque_int_pop_start(t_deque_int *ret)
{
	int	i;

	ft_assert(ret->len > 0);
	i = ret->buff[ret->start];
	ret->start = (ret->start + 1) % ret->cap;
	ret->len--;
	return (i);
}
