#ifndef BOUNDS_H
# define BOUNDS_H
# include "mymath.h"
# include <stdint.h>
# include "math.h"
typedef struct s_bounds3f {
	t_fvec3 min;
	t_fvec3 max;
} t_bounds3f;

static const t_bounds3f BOUNDS_DEGENERATE = {{NAN, NAN, NAN}, {NAN, NAN, NAN}};

t_bounds3f bounds_extend_pt(t_bounds3f bounds, t_fvec3 pt);
t_bounds3f bounds_extend_bounds(t_bounds3f b1, t_bounds3f b2);
t_fvec3 bounds_centroid(t_bounds3f bounds);
float bounds_surphace_area(t_bounds3f bounds);
uint8_t bounds_max_dim(t_bounds3f bounds);
t_fvec3 bounds_offset(t_bounds3f bounds, t_fvec3 p);
#endif
