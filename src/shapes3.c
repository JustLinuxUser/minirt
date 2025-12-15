/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shapes3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 23:39:04 by anddokhn          #+#    #+#             */
/*   Updated: 2025/12/15 23:39:04 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shapes.h"
#include "float_utils.h"

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
