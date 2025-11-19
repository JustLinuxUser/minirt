/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounds2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 10:18:42 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 10:18:42 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bounds.h"
#include "float_utils.h"

uint8_t	bounds_max_dim(t_bounds3f bounds)
{
	t_fvec3	diag;

	diag = fvec3_sub(bounds.max, bounds.min);
	if (diag.x >= diag.y && diag.x >= diag.z)
		return (0);
	if (diag.y >= diag.x && diag.y >= diag.z)
		return (1);
	if (diag.z >= diag.x && diag.z >= diag.y)
		return (2);
	return (2);
}

t_fvec3	bounds_offset(t_bounds3f bounds, t_fvec3 p)
{
	return ((t_fvec3){
		.x = fmax((p.x - bounds.min.x) / (bounds.max.x - bounds.min.x), 0),
		.y = fmax((p.y - bounds.min.y) / (bounds.max.y - bounds.min.y), 0),
		.z = fmax((p.z - bounds.min.z) / (bounds.max.z - bounds.min.z), 0),
	});
}

bool	bounds_check_enclosed(t_bounds3f big, t_bounds3f small)
{
	if (big.min.x > small.min.x || big.min.y > small.min.y
		|| big.min.z > small.min.z || big.max.x < small.max.x
		|| big.max.y < small.max.y || big.max.z < small.max.z)
		return (false);
	return (true);
}

bool	intersect_bounds(t_bounds3f bounds,
			t_ray r,
			float t_max,
			float *hitt0)
{
	t_fvec2	ts;
	float	inv_ray_dir;
	float	t_near;
	float	t_far;
	int		i;

	ts.x = 0;
	ts.y = t_max;
	i = -1;
	while (++i < 3)
	{
		inv_ray_dir = 1 / fvec3_idx(r.dir, i);
		t_near = (fvec3_idx(bounds.min, i) - fvec3_idx(r.pos, i)) * inv_ray_dir;
		t_far = (fvec3_idx(bounds.max, i) - fvec3_idx(r.pos, i)) * inv_ray_dir;
		sort2f(&t_near, &t_far);
		t_far *= 1 + 2 * f32_gamma(3);
		ts.x = fmax(t_near, ts.x);
		ts.y = fmin(t_far, ts.y);
		if (ts.x > ts.y)
			return (false);
	}
	if (hitt0)
		*hitt0 = ts.x;
	return (true);
}
