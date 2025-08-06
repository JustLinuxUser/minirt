/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:36:22 by anddokhn          #+#    #+#             */
/*   Updated: 2025/01/23 19:14:01 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>
#include <stdint.h>

int	deque_init(t_deque *ret, size_t size)
{
	ret->end = -1;
	ret->start = 0;
	ret->len = 0;
	ret->cap = size;
	ret->buff = malloc(size);
	ret->initialized = 1;
	ret->finished = 0;
	if (ret->buff == 0)
		return (1);
	return (0);
}

int	deque_double(t_deque *old)
{
	t_deque	new;

	if (deque_init(&new, old->cap * 2))
		return (1);
	while (old->len)
	{
		new.len++;
		new.end++;
		new.end %= new.cap;
		old->len--;
		new.buff[new.end] = old->buff[old->start];
		old->start = (old->start + 1) % old->cap;
	}
	free(old->buff);
	old->buff = new.buff;
	old->len = new.len;
	old->cap = new.cap;
	old->start = new.start;
	old->end = new.end;
	return (0);
}

int	deque_find_newline(t_deque q)
{
	int	i;

	i = 0;
	while (i < q.len)
	{
		if (q.buff[(q.start + i) % q.cap] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

int	deque_get_line(t_deque *q, char **ret)
{
	uint32_t	size;
	int			i;

	if (q->len == 0)
		return (0);
	size = deque_find_newline(*q) + 1;
	if (size == 0)
		size = q->len;
	*ret = malloc(size + 1);
	if (*ret == 0)
		return (1);
	i = 0;
	while ((uint32_t)i < size)
	{
		q->len--;
		(*ret)[i] = q->buff[q->start];
		q->start = (q->start + 1) % q->cap;
		i++;
	}
	(*ret)[i] = 0;
	return (0);
}

int	deque_push(t_deque *q, char c)
{
	if (q->len == q->cap)
		if (deque_double(q))
			return (1);
	q->len++;
	q->end++;
	q->end %= q->cap;
	q->buff[q->end] = c;
	return (0);
}
