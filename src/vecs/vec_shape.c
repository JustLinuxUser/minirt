/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_shape.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:51:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/03/17 19:15:09 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "vec_shape.h"
#include "../libft/utils/utils.h"

int	vec_shape_init(t_vec_shape *ret, size_t size)
{
	*ret = (t_vec_shape){.cap = size, .buff = malloc(size * sizeof(t_shape))};
	return (0);
}

int	vec_shape_double(t_vec_shape *v)
{
	t_shape	*temp;
	size_t	i;

	v->cap = v->cap * 2;
	if (!v->cap)
		v->cap = 64;
	temp = malloc(sizeof(t_shape) * v->cap);
	if (temp == 0)
		return (1);
	i = -1;
	while (++i < v->len)
		temp[i] = v->buff[i];
	free(v->buff);
	v->buff = temp;
	return (0);
}

int	vec_shape_push(t_vec_shape *v, t_shape el)
{
	if (v->len == v->cap)
		if (vec_shape_double(v))
			return (1);
	v->buff[v->len++] = el;
	return (0);
}

t_shape	vec_shape_pop(t_vec_shape *v)
{
	ft_assert(v->len > 0);
	return (v->buff[--v->len]);
}

t_shape	vec_shape_idx(t_vec_shape *v, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff[idx]);
}
