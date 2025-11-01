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
#ifndef VEC_SPHERE_H
# define VEC_SPHERE_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_sphere
{
	size_t			cap;
	size_t			len;
	t_sphere		*buff;
}	t_vec_sphere;

inline static bool	vec_sphere_init(
	t_vec_sphere *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_sphere), init_alloc));
}

inline static bool	vec_sphere_push(
	t_vec_sphere *v, t_sphere el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_sphere), &el));
}

inline static t_sphere	vec_sphere_pop(
	t_vec_sphere *v)
{
	t_sphere	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_sphere), &el));
	return (el);
}

inline static t_sphere	*vec_sphere_idx(
	t_vec_sphere *v, size_t idx)
{
	return ((t_sphere *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_sphere), idx));
}
#endif
