/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_dyn_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:51:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/04/16 13:06:21 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "vec_dyn_str.h"
#include "../utils/utils.h"

int	vec_dyn_str_init(t_vec_dyn_str *ret)
{
	*ret = (t_vec_dyn_str){0};
	return (0);
}

int	vec_dyn_str_double(t_vec_dyn_str *v)
{
	t_dyn_str	*temp;
	size_t		i;

	v->cap *= 2;
	if (v->cap == 0)
		v->cap = 64 / sizeof(char *);
	temp = malloc(sizeof(t_dyn_str) * v->cap);
	if (temp == 0)
		return (1);
	i = -1;
	while (++i < v->len)
		temp[i] = v->buff[i];
	free(v->buff);
	v->buff = temp;
	return (0);
}

int	vec_dyn_str_push(t_vec_dyn_str *v, t_dyn_str el)
{
	if (v->len == v->cap)
		if (vec_dyn_str_double(v))
			return (1);
	v->buff[v->len++] = el;
	return (0);
}

t_dyn_str	vec_dyn_str_pop(t_vec_dyn_str *v)
{
	ft_assert(v->len > 0);
	return (v->buff[--v->len]);
}

t_dyn_str	vec_dyn_str_idx(t_vec_dyn_str *v, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff[idx]);
}
