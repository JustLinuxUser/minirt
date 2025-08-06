/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_fvec3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:51:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/03/17 19:15:09 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "vec_fvec3.h"
#include "../libft/utils/utils.h"

int	vec_fvec3_init(t_vec_fvec3 *ret)
{
	*ret = (t_vec_fvec3){0};
	return (0);
}

int	vec_fvec3_double(t_vec_fvec3 *v)
{
	t_fvec3	*temp;
	size_t	i;

	v->cap = v->cap * 2 + 1;
	temp = malloc(sizeof(t_fvec3) * v->cap);
	if (temp == 0)
		return (1);
	i = -1;
	while (++i < v->len)
		temp[i] = v->buff[i];
	free(v->buff);
	v->buff = temp;
	return (0);
}

int	vec_fvec3_push(t_vec_fvec3 *v, t_fvec3 el)
{
	if (v->len == v->cap)
		if (vec_fvec3_double(v))
			return (1);
	v->buff[v->len++] = el;
	return (0);
}

t_fvec3	vec_fvec3_pop(t_vec_fvec3 *v)
{
	ft_assert(v->len > 0);
	return (v->buff[--v->len]);
}

t_fvec3	vec_fvec3_idx(t_vec_fvec3 *v, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff[idx]);
}
