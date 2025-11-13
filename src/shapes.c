/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:54:31 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 20:54:43 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shapes.h"
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include "bounds.h"
#include "libft/utils/utils.h"
#include "minirt.h"

bool	intersect_sphere(t_sphere s, t_ray r, float *t, bool *inside)
{
	t_fvec3	l;
	float	tc;
	t_fvec2	ts;
	float	d;
	float	tlc;

	l = fvec3_sub(s.p, r.pos);
	tc = fvec3_dot(l, fvec3_norm(r.dir));
	d = fvec3_dot(l, l) - tc * tc;
	if (d > s.r * s.r || d < 0)
		return (false);
	tlc = sqrt(s.r * s.r - d);
	ts.x = tc + tlc;
	ts.y = tc - tlc;
	if ((ts.x < ts.y && ts.x > 0) || ts.y < 0)
		*t = ts.x;
	else
		*t = ts.y;
	*inside = ts.x > 0 && ts.y > 0;
	return (true);
}

typedef struct s_isect_cylinder
{
	t_fvec3	ba;
	t_fvec3	oc;
	float	baba;
	float	bard;
	float	baoc;
	float	k2;
	float	k1;
	float	h;
	float	t;
	float	y;
}	t_isect_cylinder;

bool	intersect_cylinder_caps(t_isect_cylinder i, float *t_ret, t_fvec3 *norm)
{
	i.t = (((i.y >= 0.0) * i.baba) - i.baoc) / i.bard;
	if (fabs(i.k1 + i.k2 * i.t) < i.h)
	{
		*t_ret = i.t;
		*norm = fvec3_norm(i.ba);
		return (true);
	}
	return (false);
}

// https://iquilezles.org/articles/intersectors/
bool	intersect_cylinder(t_ray r, t_cylinder cy, float *t_ret, t_fvec3 *norm)
{
	t_isect_cylinder	i;

	i.ba = fvec3_sub(cy.b, cy.a);
	i.oc = fvec3_sub(r.pos, cy.a);
	i.baba = fvec3_dot(i.ba, i.ba);
	i.bard = fvec3_dot(i.ba, r.dir);
	i.baoc = fvec3_dot(i.ba, i.oc);
	i.k2 = i.baba - i.bard * i.bard;
	i.k1 = i.baba * fvec3_dot(i.oc, r.dir) - i.baoc * i.bard;
	i.h = i.k1 * i.k1 - i.k2 * (i.baba * fvec3_dot(i.oc, i.oc)
			- i.baoc * i.baoc - cy.radius * cy.radius * i.baba);
	if (i.h < 0.0)
		return (false);
	i.h = sqrt(i.h);
	i.t = (-i.k1 - i.h) / i.k2;
	i.y = i.baoc + i.t * i.bard;
	if (i.y > 0.0 && i.y < i.baba)
	{
		*t_ret = i.t;
		*norm = fvec3_scale(fvec3_sub(fvec3_add(i.oc, fvec3_scale(r.dir, i.t)),
					fvec3_scale(fvec3_scale(i.ba, i.y), 1.0 / i.baba)),
				1 / cy.radius);
		return (true);
	}
	return (intersect_cylinder_caps(i, t_ret, norm));
}

bool	intersect_plane(t_plane plane, t_ray r, float *t)
{
	float	denom;

	denom = fvec3_dot(r.dir, plane.dir);
	if (fabs(denom) <= 1e-6)
		return (false);
	*t = fvec3_dot(fvec3_sub(plane.pos, r.pos), plane.dir) / denom;
	if (*t < 1e-6)
		return (false);
	return (true);
}

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

bool	intersect_triangle(t_ray ray,
						t_fvec3 a,
						t_fvec3 b,
						t_fvec3 c,
						t_triangle_collision *coll)
{
	t_triangle_isector	i;

	i.edge1 = fvec3_sub(b, a);
	i.edge2 = fvec3_sub(c, a);
	i.ray_cross_e2 = fvec3_cross(ray.dir, i.edge2);
	i.det = fvec3_dot(i.edge1, i.ray_cross_e2);
	if (i.det > -FLT_EPSILON && i.det < FLT_EPSILON)
		return (false);
	i.inv_det = 1.0 / i.det;
	i.s = fvec3_sub(ray.pos, a);
	coll->u = i.inv_det * fvec3_dot(i.s, i.ray_cross_e2);
	if ((coll->u < 0 && fabs(coll->u) > FLT_EPSILON)
		|| (coll->u > 1 && fabs(coll->u - 1) > FLT_EPSILON))
		return (false);
	i.s_cross_e1 = fvec3_cross(i.s, i.edge1);
	coll->v = i.inv_det * fvec3_dot(ray.dir, i.s_cross_e1);
	if ((coll->v < 0 && fabs(coll->v) > FLT_EPSILON)
		|| (coll->u + coll->v > 1 && fabs(coll->u + coll->v - 1) > FLT_EPSILON))
		return (false);
	coll->t = i.inv_det * fvec3_dot(i.edge2, i.s_cross_e1);
	return (coll->t > FLT_EPSILON);
}

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
	t_triangle_collision	coll;

	pts = triangle_points(state, triangle);
	if (intersect_triangle(isector.ray, pts.b, pts.c, pts.a, &coll))
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
	bool	inside;

	if (intersect_sphere(sphere, isector.ray, &ret->t, &inside))
	{
		ret->collided = true;
		ret->u = inside;
	}
}

inline static void	collide_shape_cylinder(t_cylinder cylinder,
		t_ray_isector isector, t_collision *ret)
{
	ret->collided = intersect_cylinder(isector.ray,
			cylinder, &ret->t, &ret->norm);
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
	if (ret.collided && (ret.t > isector.t_max
			|| (shape.ptr == isector.ignore_shape && ret.t < isector.t_min)
		))
	{
		ret.collided = false;
		ret.t = 0;
	}
	return (ret);
}

t_fvec3	collision_norm(t_state *state, t_collision collision, t_fvec3 pos)
{
	t_triangle_pts	pts;
	t_sphere		*sphere;

	if (collision.shape.type == OBJ_TRIANGLE)
	{
		pts = triangle_points(state, *(t_triangle *)collision.shape.ptr);
		return (fvec3_norm(
				fvec3_cross(fvec3_sub(pts.a, pts.b), fvec3_sub(pts.a, pts.c))));
	}
	else if (collision.shape.type == OBJ_SPHERE)
	{
		sphere = (t_sphere *)collision.shape.ptr;
		if (collision.u == 0)
			return (fvec3_invert(fvec3_norm(fvec3_sub(pos, sphere->p))));
		return (fvec3_norm(fvec3_sub(pos, sphere->p)));
	}
	else if (collision.shape.type == OBJ_CYLINDER)
		return (collision.norm);
	else if (collision.shape.type == OBJ_PLANE)
		return (((t_plane *)collision.shape.ptr)->dir);
	ft_assert("Unreachable" != 0);
	return ((t_fvec3){0});
}

t_densely_sampled_spectrum	*shape_spectrum(t_state *state, t_collision collision)
{
	int	idx;

	idx = 0;
	if (collision.shape.type == OBJ_TRIANGLE)
		idx = state->meshes.buff[(*(t_triangle *)collision.shape.ptr).mesh_idx]
			.spectrum_idx;
	else if (collision.shape.type == OBJ_SPHERE)
		idx = (*(t_sphere *)collision.shape.ptr).spectrum_idx;
	else if (collision.shape.type == OBJ_CYLINDER)
		idx = (*(t_cylinder *)collision.shape.ptr).spectrum_idx;
	else if (collision.shape.type == OBJ_PLANE)
		idx = (*(t_plane *)collision.shape.ptr).spectrum_idx;
	else
	{
		ft_assert("Unreachable" != 0);
		return (0);
	}
	return (state->spectrums.buff + idx);
}

inline static t_bounds3f	bounds_cylinder(t_cylinder cylinder)
{
	t_bounds3f	bounds;

	bounds = bounds_degenerate();
	bounds = bounds_extend_pt(bounds, fvec3_add(cylinder.a, (t_fvec3){
				.x = cylinder.radius,
				.y = cylinder.radius,
				.z = cylinder.radius}));
	bounds = bounds_extend_pt(bounds, fvec3_sub(cylinder.a, (t_fvec3){
				.x = cylinder.radius,
				.y = cylinder.radius,
				.z = cylinder.radius}));
	bounds = bounds_extend_pt(bounds, fvec3_add(cylinder.b, (t_fvec3){
				.x = cylinder.radius,
				.y = cylinder.radius,
				.z = cylinder.radius}));
	bounds = bounds_extend_pt(bounds, fvec3_sub(cylinder.b, (t_fvec3){
				.x = cylinder.radius,
				.y = cylinder.radius,
				.z = cylinder.radius}));
	return (bounds);
}

t_bounds3f	shape_bounds(t_state* state, t_shape shape)
{
	t_bounds3f		bounds;
	t_sphere		sp;
	t_triangle_pts	pts;

	bounds = bounds_degenerate();
	if (shape.type == OBJ_TRIANGLE)
	{
		pts = triangle_points(state, *(t_triangle *)shape.ptr);
		bounds = bounds_extend_pt(bounds, pts.a);
		bounds = bounds_extend_pt(bounds, pts.b);
		bounds = bounds_extend_pt(bounds, pts.c);
	}
	else if (shape.type == OBJ_SPHERE)
	{
		sp = *(t_sphere *)shape.ptr;
		bounds.min = fvec3_sub(sp.p,
				(t_fvec3){.x = sp.r, .y = sp.r, .z = sp.r});
		bounds.max = fvec3_add(sp.p,
				(t_fvec3){.x = sp.r, .y = sp.r, .z = sp.r});
	}
	else if (shape.type == OBJ_CYLINDER)
		bounds = bounds_cylinder(*(t_cylinder *)shape.ptr);
	else
		ft_assert("Unreachable" == 0);
	return (bounds);
}
