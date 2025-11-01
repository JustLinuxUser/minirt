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
#ifndef VEC_MESH_H
# define VEC_MESH_H
# include "../dsa/vec_gen.h"
# include "../utils/utils.h"

typedef struct s_vec_mesh
{
	size_t		cap;
	size_t		len;
	t_mesh		*buff;
}	t_vec_mesh;

inline static bool	vec_mesh_init(
	t_vec_mesh *ret, size_t init_alloc)
{
	return (vec_gen_init((t_vec_gen *)ret,
			sizeof(t_mesh), init_alloc));
}

inline static bool	vec_mesh_push(
	t_vec_mesh *v, t_mesh el)
{
	return (vec_gen_push((t_vec_gen *)v,
			sizeof(t_mesh), &el));
}

inline static t_mesh	vec_mesh_pop(
	t_vec_mesh *v)
{
	t_mesh	el;

	ft_assert(vec_gen_pop((t_vec_gen *)v,
			sizeof(t_mesh), &el));
	return (el);
}

inline static t_mesh	*vec_mesh_idx(
	t_vec_mesh *v, size_t idx)
{
	return ((t_mesh *)vec_gen_idx((t_vec_gen *)v,
			sizeof(t_mesh), idx));
}
#endif
