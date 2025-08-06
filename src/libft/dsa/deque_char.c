/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 00:31:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/02/05 00:23:12 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "deque_char.h"
#include "../utils/utils.h"

int	deque_char_init(t_deque_char *ret, int size)
{
	*ret = (t_deque_char){0};
	ret->buff = malloc(size * sizeof(char));
	if (ret->buff == 0)
		return (1);
	ret->cap = size;
	return (0);
}

void	deque_char_push_end(t_deque_char *ret, char item)
{
	deque_char_double_if_needed(ret);
	if (ret->len)
		ret->end = (ret->end + 1) % ret->cap;
	ret->buff[ret->end] = item;
	ret->len++;
}

void	deque_char_push_start(t_deque_char *ret, char item)
{
	deque_char_double_if_needed(ret);
	if (ret->len)
		ret->start = ft_smod(ret->start - 1, ret->cap);
	ret->buff[ret->start] = item;
	ret->len++;
}

char	deque_char_pop_end(t_deque_char *ret)
{
	char	i;

	ft_assert(ret->len > 0);
	i = ret->buff[ret->end];
	ret->len--;
	if (ret->len)
		ret->end = ft_smod(ret->end - 1, ret->cap);
	return (i);
}

char	deque_char_pop_start(t_deque_char *ret)
{
	char	i;

	ft_assert(ret->len > 0);
	i = ret->buff[ret->start];
	ret->len--;
	if (ret->len)
		ret->start = (ret->start + 1) % ret->cap;
	return (i);
}
