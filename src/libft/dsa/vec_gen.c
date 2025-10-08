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

#include "vec_gen.h"
#include <stdlib.h>
#include "../utils/utils.h"
#include "../alloc/mmalloc.h"
#include "../libft.h"

bool	vec_gen_init(t_vec_gen *ret, size_t el_size, size_t init_alloc)
{
	ft_assert(el_size != 0);
	*ret = (t_vec_gen){0};
	if (init_alloc)
		ret->buff = mmalloc(el_size * init_alloc,
				"initializing a generic vector");
	if (init_alloc && !ret->buff)
		return (false);
	return (true);
}

static bool	vec_gen_double(t_vec_gen *v, size_t el_size)
{
	void	*temp;

	v->cap *= 2;
	if (v->cap == 0)
		v->cap = 64;
	temp = mmalloc(el_size * v->cap, "resizing a generic vector");
	if (!temp)
		return (false);
	ft_memcpy(temp, v->buff, v->len * el_size);
	free(v->buff);
	v->buff = temp;
	return (true);
}

bool	vec_gen_push(t_vec_gen *v, size_t el_size, void *el)
{
	if (v->len == v->cap)
		if (!vec_gen_double(v, el_size))
			return (false);
	ft_memcpy(v->buff + (v->len++) * el_size, el, el_size);
	return (true);
}

bool	vec_gen_pop(t_vec_gen *v, size_t el_size, void *ret)
{
	if (v->len == 0)
		return (false);
	ft_memcpy(ret, v->buff + (--v->len) * el_size, el_size);
	return (true);
}

void	*vec_gen_idx(t_vec_gen *v, size_t el_size, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff + idx * el_size);
}
