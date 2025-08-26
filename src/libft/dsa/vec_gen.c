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
#include "vec_gen.h"
#include "../utils/utils.h"
#include "../alloc/mmalloc.h"
#include "../libft.h"

int	vec_gen_init(t_vec_gen *ret, size_t el_size, size_t init_alloc)
{
	ft_assert(el_size != 0);
	*ret = (t_vec_gen){.el_size = el_size};
	if (init_alloc)
		ret->buff = mmalloc(el_size * init_alloc,
				"initializing a generic vector");
	if (init_alloc && !ret->buff)
		return (1);
	return (0);
}

int	vec_gen_double(t_vec_gen *v)
{
	void	*temp;

	v->cap *= 2;
	if (v->cap == 0)
		v->cap = 64;
	temp = mmalloc(v->el_size * v->cap, "resizing a generic vector");
	if (!temp)
		return (1);
	ft_memcpy(temp, v->buff, v->len * v->el_size);
	free(v->buff);
	v->buff = temp;
	return (0);
}

int	vec_gen_push(t_vec_gen *v, void *el)
{
	if (v->len == v->cap)
		if (vec_gen_double(v))
			return (1);
	ft_memcpy(v->buff + v->len++, el, v->el_size);
	return (0);
}

int	vec_gen_pop(t_vec_gen *v, void *ret)
{
	if (v->len == 0)
		return (1);
	ft_memcpy(ret, v->buff + --v->len, v->el_size);
	return (0);
}

void	*vec_gen_idx(t_vec_gen *v, size_t idx)
{
	ft_assert(idx < v->len);
	return (v->buff + idx);
}
