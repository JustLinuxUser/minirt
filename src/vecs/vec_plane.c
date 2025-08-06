/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_plane.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:51:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/03/17 19:15:09 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "vec_plane.h"
#include "../libft/utils/utils.h"

int	vec_plane_init(t_vec_plane *ret)
{
	*ret = (t_vec_plane){0};
	return (0);
}

int	vec_plane_double(t_vec_plane *v)
{
	t_plane	*temp;
	size_t	i;

	v->cap = v->cap * 2;
	if (!v->cap)
		v->cap = 64;
	temp = malloc(sizeof(t_plane) * v->cap);
	if (temp == 0)
		return (1);
	i = -1;
	while (++i < v->len)
		temp[i] = v->buff[i];
	free(v->buff);
	v->buff = temp;
	return (0);
}

int	vec_plane_push(t_vec_plane *v, t_plane el)
{
	if (v->len == v->cap)
		if (vec_plane_double(v))
			return (1);
	v->buff[v->len++] = el;
	return (0);
}

t_plane	vec_plane_pop(t_vec_plane *v)
{
	ft_assert(v->len > 0);
	return (v->buff[--v->len]);
}

t_plane	vec_plane_idx(t_vec_plane *v, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff[idx]);
}
