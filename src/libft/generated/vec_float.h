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
#ifndef VEC_FLOAT_H
# define VEC_FLOAT_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_float
{
	size_t		cap;
	size_t		len;
	float		*buff;
}	t_vec_float;

inline static bool	vec_float_init(
	t_vec_float *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(float), init_alloc));
}

inline static bool	vec_float_push(
	t_vec_float *v, float el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(float), &el));
}

inline static float	vec_float_pop(
	t_vec_float *v)
{
	float	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(float), &el));
	return (el);
}

inline static float	*vec_float_idx(
	t_vec_float *v, size_t idx)
{
	return ((float *)vec_gen_idx((t_vec_gen *)v,
			sizeof(float), idx));
}
#endif
