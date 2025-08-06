/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_rt_nd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:51:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/03/17 19:15:09 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "rt_parser.h"
#include "../libft/utils/utils.h"

int	vec_rt_nd_init(t_vec_rt_nd *ret)
{
	*ret = (t_vec_rt_nd){0};
	return (0);
}

int	vec_rt_nd_double(t_vec_rt_nd *v)
{
	t_rt_node	*temp;
	size_t	i;

	v->cap = v->cap * 2 + 1;
	temp = malloc(sizeof(t_rt_node) * v->cap);
	if (temp == 0)
		return (1);
	i = -1;
	while (++i < v->len)
		temp[i] = v->buff[i];
	free(v->buff);
	v->buff = temp;
	return (0);
}

int	vec_rt_nd_push(t_vec_rt_nd *v, t_rt_node el)
{
	if (v->len == v->cap)
		if (vec_rt_nd_double(v))
			return (1);
	v->buff[v->len++] = el;
	return (0);
}

t_rt_node	vec_rt_nd_pop(t_vec_rt_nd *v)
{
	ft_assert(v->len > 0);
	return (v->buff[--v->len]);
}

t_rt_node	vec_rt_nd_idx(t_vec_rt_nd *v, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff[idx]);
}
