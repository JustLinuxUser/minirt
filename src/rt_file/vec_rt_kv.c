/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_rt_kv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:51:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/03/17 19:15:09 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "rt_parser.h"

int	vec_rt_kv_init(t_vec_rt_kv *ret)
{
	*ret = (t_vec_rt_kv){0};
	return (0);
}

int	vec_rt_kv_double(t_vec_rt_kv *v)
{
	t_rt_kv	*temp;
	size_t	i;

	v->cap = v->cap * 2 + 1;
	temp = malloc(sizeof(t_rt_kv) * v->cap);
	if (temp == 0)
		return (1);
	i = -1;
	while (++i < v->len)
		temp[i] = v->buff[i];
	free(v->buff);
	v->buff = temp;
	return (0);
}

int	vec_rt_kv_push(t_vec_rt_kv *v, t_rt_kv el)
{
	if (v->len == v->cap)
		if (vec_rt_kv_double(v))
			return (1);
	v->buff[v->len++] = el;
	return (0);
}

t_rt_kv	vec_rt_kv_pop(t_vec_rt_kv *v)
{
	ft_assert(v->len > 0);
	return (v->buff[--v->len]);
}

t_rt_kv	vec_rt_kv_idx(t_vec_rt_kv *v, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff[idx]);
}
