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
#ifndef VEC_PLANE_H
# define VEC_PLANE_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_plane
{
	size_t		cap;
	size_t		len;
	t_plane		*buff;
}	t_vec_plane;

inline static bool	vec_plane_init(
	t_vec_plane *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_plane), init_alloc));
}

inline static bool	vec_plane_push(
	t_vec_plane *v, t_plane el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_plane), &el));
}

inline static t_plane	vec_plane_pop(
	t_vec_plane *v)
{
	t_plane	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_plane), &el));
	return (el);
}

inline static t_plane	*vec_plane_idx(
	t_vec_plane *v, size_t idx)
{
	return ((t_plane *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_plane), idx));
}
#endif
