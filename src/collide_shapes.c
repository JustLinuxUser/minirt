/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collide_shapes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 14:37:56 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/15 14:37:56 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include "shapes.h"

t_triangle_pts	triangle_points(t_state *state, t_triangle triangle)
{
	t_mesh			*mesh;
	int				*vidxs;
	t_triangle_pts	ret;

	mesh = &state->meshes.buff[triangle.mesh_idx];
	vidxs = &mesh->vertex_idxs.buff[triangle.triangle_idx * 3];
	ret.a = mesh->vertices.buff[vidxs[0]];
	ret.b = mesh->vertices.buff[vidxs[1]];
	ret.c = mesh->vertices.buff[vidxs[2]];
	return (ret);
}

inline static void	collide_shape_triangle(t_state *state, t_triangle triangle,
		t_ray_isector isector, t_collision *ret)
{
	t_triangle_pts			pts;
	t_triangle_pts			pts_param;
	t_triangle_collision	coll;

	pts = triangle_points(state, triangle);
	pts_param.a = pts.b;
	pts_param.b = pts.c;
	pts_param.c = pts.a;
	if (intersect_triangle(isector.ray, pts_param, &coll))
	{
		ret->collided = true;
		ret->t = coll.t;
		ret->u = coll.u;
		ret->v = coll.v;
	}
}

inline static void	collide_shape_sphere(t_sphere sphere,
		t_ray_isector isector, t_collision *ret)
{
	ret->collided = intersect_sphere(sphere, isector.ray, &ret->t);
}

inline static void	collide_shape_cylinder(t_cylinder cylinder,
		t_ray_isector isector, t_collision *ret)
{
	ret->collided = intersect_cylinder(isector.ray,
			cylinder, &ret->t, (bool *)&ret->u);
}

t_collision	collide_shape(t_state *state, t_shape shape, t_ray_isector isector)
{
	t_collision	ret;

	ret = (t_collision){.shape = shape};
	if (shape.type == OBJ_TRIANGLE)
		collide_shape_triangle(state, *(t_triangle *)shape.ptr, isector, &ret);
	else if (shape.type == OBJ_SPHERE)
		collide_shape_sphere(*(t_sphere *)shape.ptr, isector, &ret);
	else if (shape.type == OBJ_CYLINDER)
		collide_shape_cylinder(*(t_cylinder *)shape.ptr, isector, &ret);
	else if (shape.type == OBJ_PLANE)
		ret.collided = intersect_plane(*(t_plane *)shape.ptr,
				isector.ray, &ret.t);
	else
		ft_assert("Unreachable" != 0);
	return (ret);
}
