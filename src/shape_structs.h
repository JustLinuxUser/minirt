/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 12:04:20 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/15 12:04:20 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPE_STRUCTS_H
# define SHAPE_STRUCTS_H

struct	s_state;

enum e_obj_type
{
	OBJ_SPHERE,
	OBJ_PLANE,
	OBJ_CYLINDER,
	OBJ_TRIANGLE,
};

typedef struct s_triangle
{
	int	mesh_idx;
	int	triangle_idx;
}	t_triangle;

typedef struct t_sphere
{
	t_fvec3	p;
	float	r;
	int		spectrum_idx;
}	t_sphere;

typedef struct t_cylinder
{
	t_fvec3	b;
	t_fvec3	dir;
	float	radius;
	float	height;
	int		spectrum_idx;
}	t_cylinder;

typedef struct s_mesh
{
	t_vec_fvec3	vertices;
	t_vec_int	vertex_idxs;
	int			spectrum_idx;
}	t_mesh;

typedef struct s_plane
{
	t_fvec3	pos;
	t_fvec3	dir;
	int		spectrum_idx;
}	t_plane;

typedef struct s_shape
{
	enum e_obj_type	type;
	void			*ptr;
}	t_shape;
#endif
