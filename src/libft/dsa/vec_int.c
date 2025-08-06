/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_int.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:51:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/04/16 13:07:05 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "vec_int.h"
#include "../utils/utils.h"

int	vec_int_init(t_vec_int *ret)
{
	*ret = (t_vec_int){0};
	return (0);
}

int	vec_int_double(t_vec_int *v)
{
	int		*temp;
	size_t	i;

	v->cap *= 2;
	if (v->cap == 0)
		v->cap = 64 / sizeof(v->cap);
	temp = malloc(sizeof(int) * v->cap);
	if (temp == 0)
		return (1);
	i = -1;
	while (++i < v->len)
		temp[i] = v->buff[i];
	free(v->buff);
	v->buff = temp;
	return (0);
}

int	vec_int_push(t_vec_int *v, int el)
{
	if (v->len == v->cap)
		if (vec_int_double(v))
			return (1);
	v->buff[v->len++] = el;
	return (0);
}

int	vec_int_pop(t_vec_int *v)
{
	ft_assert(v->len > 0);
	return (v->buff[--v->len]);
}

int	vec_int_idx(t_vec_int *v, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff[idx]);
}
