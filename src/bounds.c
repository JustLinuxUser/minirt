#include "bounds.h"
#include <math.h>
#include <stdint.h>

t_bounds3f bounds_extend_pt(t_bounds3f bounds, t_fvec3 pt) {
	bounds.min.x = fmin(bounds.min.x, pt.x);
	bounds.min.y = fmin(bounds.min.y, pt.y);
	bounds.min.z = fmin(bounds.min.z, pt.z);

	bounds.max.x = fmax(bounds.max.x, pt.x);
	bounds.max.y = fmax(bounds.max.y, pt.y);
	bounds.max.z = fmax(bounds.max.z, pt.z);
	return (bounds);
}

t_bounds3f bounds_extend_bounds(t_bounds3f b1, t_bounds3f b2) {
	b1 = bounds_extend_pt(b1, b2.min);
	b1 = bounds_extend_pt(b1, b2.max);
	return (b1);
}

t_fvec3 bounds_centroid(t_bounds3f bounds) {
	return fvec3_add(bounds.min, fvec3_scale(fvec3_sub(bounds.max, bounds.min), 0.5));
}

float bounds_surphace_area(t_bounds3f bounds) {
	t_fvec3 diag = fvec3_sub(bounds.max, bounds.min);
	return 2 * (diag.x * diag.y + diag.x * diag.z + diag.y * diag.z);
}

uint8_t bounds_max_dim(t_bounds3f bounds) {
	t_fvec3 diag = fvec3_sub(bounds.max, bounds.min);
	if (diag.x >= diag.y && diag.x >= diag.z)
		return (0);
	if (diag.y >= diag.x && diag.y >= diag.z)
		return (1);
	if (diag.z >= diag.x && diag.z >= diag.y)
		return (2);
	return (2);
}

t_fvec3 bounds_offset(t_bounds3f bounds, t_fvec3 p)
{
	return (t_fvec3) {
		.x = fmax((p.x - bounds.min.x) / (bounds.max.x - bounds.min.x), 0),
		.y = fmax((p.y - bounds.min.y) / (bounds.max.y - bounds.min.y), 0),
		.z = fmax((p.z - bounds.min.z) / (bounds.max.z - bounds.min.z), 0),
	};
}
