/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:45:12 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 20:51:46 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPES_H
# define SHAPES_H
# include "bounds.h"
# include "light_structs.h"
# include "ray.h"
# include "mymath.h"
# include <stdbool.h>
# include "libft/generated/vec_fvec3.h"
# include "libft/generated/vec_int.h"
# include "shape_structs.h"
# include "libft/generated/vec_sphere.h"
# include "libft/generated/vec_cylinder.h"
# include "libft/generated/vec_plane.h"
# include "libft/generated/vec_mesh.h"
# include "libft/generated/vec_shape.h"
# include "libft/generated/vec_triangle.h"

typedef struct s_collision
{
	float	t;
	bool	collided;
	bool	disk_collision; //  cylinder
	/*triangle*/
	float	u;
	float	v;
	t_shape	shape;
}	t_collision;

typedef struct s_triangle_collision
{
	float	t;
	float	u;
	float	v;
	t_shape	shape;
}	t_triangle_collision;

typedef struct s_triangle_isector
{
	float	inv_det;
	float	det;
	t_fvec3	edge1;
	t_fvec3	edge2;
	t_fvec3	ray_cross_e2;
	t_fvec3	s_cross_e1;
	t_fvec3	s;
}	t_triangle_isector;

typedef struct s_isect_cylinder
{
	t_fvec3	b;
	t_fvec3	a;
	t_fvec3	n;
	t_fvec3	nxa;
	float	nxa_len_sq;
	t_fvec3	bxa;
	float	rhs;
}	t_isect_cylinder;

typedef struct t_triangle_pts
{
	t_fvec3	a;
	t_fvec3	b;
	t_fvec3	c;
}	t_triangle_pts;

t_collision					collide_shape(struct s_state *state,
								t_shape shape, t_ray_isector isector);
t_fvec3						collision_norm(struct s_state *state,
								t_collision collision, t_fvec3 pos);
t_bounds3f					shape_bounds(struct s_state *state, t_shape shape);
t_densely_sampled_spectrum	*shape_spectrum(struct s_state *state,
								t_collision collision);
bool						intersect_sphere(t_sphere s, t_ray r,
								float *t);
bool						intersect_cylinder(t_ray r, t_cylinder cy,
								float *t_ret, bool *cap);
bool						intersect_plane(t_plane plane, t_ray r, float *t);
bool						intersect_triangle(t_ray ray,
								t_triangle_pts pts,
								t_triangle_collision *coll);
t_triangle_pts				triangle_points(struct s_state *state,
								t_triangle triangle);
#endif
