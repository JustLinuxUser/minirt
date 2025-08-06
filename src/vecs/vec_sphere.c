/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:51:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/03/17 19:15:09 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "vec_sphere.h"
#include "../libft/utils/utils.h"

int	vec_sphere_init(t_vec_sphere *ret)
{
	*ret = (t_vec_sphere){0};
	return (0);
}

int	vec_sphere_double(t_vec_sphere *v)
{
	t_sphere	*temp;
	size_t	i;

	v->cap = v->cap * 2;
	if (!v->cap)
		v->cap = 64;
	temp = malloc(sizeof(t_sphere) * v->cap);
	if (temp == 0)
		return (1);
	i = -1;
	while (++i < v->len)
		temp[i] = v->buff[i];
	free(v->buff);
	v->buff = temp;
	return (0);
}

int	vec_sphere_push(t_vec_sphere *v, t_sphere el)
{
	if (v->len == v->cap)
		if (vec_sphere_double(v))
			return (1);
	v->buff[v->len++] = el;
	return (0);
}

t_sphere	vec_sphere_pop(t_vec_sphere *v)
{
	ft_assert(v->len > 0);
	return (v->buff[--v->len]);
}

t_sphere	vec_sphere_idx(t_vec_sphere *v, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff[idx]);
}
