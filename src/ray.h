#ifndef BVH_H
# define BVH_H
# include "mymath.h"

typedef struct t_ray_isector {
	t_ray   ray;
	float	t_min;
	float	t_max;
	void	*ignore_shape;
} t_ray_isector;

#endif
