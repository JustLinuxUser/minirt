/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*                                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 15:11:48 by anddokhn          #+#    #+#             */
/*   Updated: 2025/04/08 22:26:02 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef VEC_SHAPE_H
# define VEC_SHAPE_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_shape
{
	size_t		cap;
	size_t		len;
	t_shape		*buff;
}	t_vec_shape;

inline static bool	vec_shape_init(
	t_vec_shape *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_shape), init_alloc));
}

inline static bool	vec_shape_push(
	t_vec_shape *v, t_shape el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_shape), &el));
}

inline static t_shape	vec_shape_pop(
	t_vec_shape *v)
{
	t_shape	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_shape), &el));
	return (el);
}

inline static t_shape	*vec_shape_idx(
	t_vec_shape *v, size_t idx)
{
	return ((t_shape *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_shape), idx));
}
#endif
