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
#include "float_utils.h"
#include <math.h>
#include <stdbool.h>

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
	return (*t > 0);
}

bool	intersect_cylinder_caps(t_isect_cylinder i, float *t_ret, t_fvec3 *norm)
{
	i.t = (((i.y >= 0.0) * i.baba) - i.baoc) / i.bard;
	if (fabs(i.k1 + i.k2 * i.t) < i.h)
	{
		*t_ret = i.t;
		*norm = fvec3_norm(i.ba);
		return (*t_ret > 0);
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
		return (*t_ret > 0);
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

bool	intersect_triangle(t_ray ray,
						t_triangle_pts pts,
						t_triangle_collision *coll)
{
	t_triangle_isector	i;

	i.edge1 = fvec3_sub(pts.b, pts.a);
	i.edge2 = fvec3_sub(pts.c, pts.a);
	i.ray_cross_e2 = fvec3_cross(ray.dir, i.edge2);
	i.det = fvec3_dot(i.edge1, i.ray_cross_e2);
	if (i.det > -FLT_EPSILON && i.det < FLT_EPSILON)
		return (false);
	i.inv_det = 1.0 / i.det;
	i.s = fvec3_sub(ray.pos, pts.a);
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
