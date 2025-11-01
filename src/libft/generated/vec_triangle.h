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
#ifndef VEC_TRIANGLE_H
# define VEC_TRIANGLE_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_triangle
{
	size_t			cap;
	size_t			len;
	t_triangle		*buff;
}	t_vec_triangle;

inline static bool	vec_triangle_init(
	t_vec_triangle *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_triangle), init_alloc));
}

inline static bool	vec_triangle_push(
	t_vec_triangle *v, t_triangle el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_triangle), &el));
}

inline static t_triangle	vec_triangle_pop(
	t_vec_triangle *v)
{
	t_triangle	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_triangle), &el));
	return (el);
}

inline static t_triangle	*vec_triangle_idx(
	t_vec_triangle *v, size_t idx)
{
	return ((t_triangle *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_triangle), idx));
}
#endif
