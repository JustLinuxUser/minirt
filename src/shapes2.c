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

#include "mymath.h"
#include "shapes.h"
#include <math.h>
#include <stdbool.h>

bool	intersect_sphere(t_sphere s, t_ray r, float *t)
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
	*t = NAN;
	if (ts.x > 0.01 && fminf(*t, ts.x) == ts.x)
		*t = ts.x;
	if (ts.y > 0.01 && fminf(*t, ts.y) == ts.y)
		*t = ts.y;
	return (!isnan(*t));
}

void	best_cylinder_t(float *t, float new_t, t_ray r, t_cylinder cy)
{
	t_fvec3	b;
	t_fvec3	a;
	float	coll_height;

	b = fvec3_sub(cy.b, r.pos);
	a = cy.dir;
	if (!(new_t > 0.01 && fminf(new_t, *t) == new_t))
		return ;
	coll_height = fvec3_dot(a, fvec3_sub(fvec3_scale(r.dir, new_t), b));
	if (coll_height >= -0.01 && coll_height <= cy.height + 0.01)
		*t = new_t;
}

bool	best_cylinder_cap(float *t, t_fvec3 c, t_ray r, t_cylinder cy)
{
	t_fvec3	a;
	float	new_t;
	float	dist_sq;

	a = cy.dir;
	new_t = fvec3_dot(a, c) / fvec3_dot(a, r.dir);
	if (!(new_t > 0.01 && fminf(new_t, *t) == new_t))
		return (false);
	dist_sq = fvec3_len_sq(fvec3_sub(fvec3_scale(r.dir, new_t), c));
	if (dist_sq <= cy.radius * cy.radius)
	{
		*t = new_t;
		return (true);
	}
	return (false);
}

bool	intersect_cylinder(t_ray r, t_cylinder cy, float *t_ret, bool *cap)
{
	t_isect_cylinder	i;

	i.b = fvec3_sub(cy.b, r.pos);
	i.a = cy.dir;
	i.n = r.dir;
	i.nxa = fvec3_cross(i.n, i.a);
	i.nxa_len_sq = fvec3_len_sq(i.nxa);
	if (i.nxa_len_sq == 0)
		return (false);
	i.bxa = fvec3_cross(i.b, i.a);
	i.rhs = i.nxa_len_sq * cy.radius * cy.radius
		- 1 * powf(fvec3_dot(i.b, i.nxa), 2);
	if (i.rhs < 0)
		return (false);
	*t_ret = NAN;
	best_cylinder_t(t_ret,
		(fvec3_dot(i.bxa, i.nxa) - sqrtf(i.rhs)) / i.nxa_len_sq, r, cy);
	best_cylinder_t(t_ret,
		(fvec3_dot(i.bxa, i.nxa) + sqrtf(i.rhs)) / i.nxa_len_sq, r, cy);
	*cap = false;
	*cap = best_cylinder_cap(t_ret, i.b, r, cy);
	*cap = *cap || best_cylinder_cap(t_ret,
			fvec3_add(i.b, fvec3_scale(i.a, cy.height)), r, cy);
	return (!isnan(*t_ret));
}

bool	intersect_plane(t_plane plane, t_ray r, float *t)
{
	float	denom;

	denom = fvec3_dot(r.dir, plane.dir);
	if (fabs(denom) <= 1e-6)
		return (false);
	*t = fvec3_dot(fvec3_sub(plane.pos, r.pos), plane.dir) / denom;
	return (true);
}
