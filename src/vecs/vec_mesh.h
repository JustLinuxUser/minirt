/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_mesh.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:52:15 by anddokhn          #+#    #+#             */
/*   Updated: 2025/01/14 00:34:37 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_MESH_H
# define VEC_MESH_H
# include <stdlib.h>
# include "../mymath.h"
#include "vec_fvec3.h"
#include "vec_int.h"

typedef struct s_mesh {
	t_vec_fvec3	vertices;
	t_vec_int	vertex_idxs;
} t_mesh;

typedef struct s_vec_mesh
{
	size_t	cap;
	size_t	len;
	t_mesh		*buff;
}	t_vec_mesh;
int	vec_mesh_init(t_vec_mesh *ret);
int	vec_mesh_double(t_vec_mesh *v);
int	vec_mesh_push(t_vec_mesh *v, t_mesh el);
t_mesh	vec_mesh_pop(t_vec_mesh *v);
t_mesh	vec_mesh_idx(t_vec_mesh *v, size_t idx);
#endif
