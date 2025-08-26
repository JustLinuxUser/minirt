#ifndef SHAPES_H
# define SHAPES_H
#include "bounds.h"
#include "mymath.h"
#include <stdbool.h>

typedef struct s_state t_state;

enum obj_type {
    OBJ_SPHERE,
    OBJ_PLANE,
    OBJ_TRIANGLE,
};

typedef struct s_shape {
    enum obj_type type;
	void *ptr;
} t_shape;

typedef struct s_collision {
    bool collided;
    float t;

    // triangle
    float u;
    float v;

	t_shape shape;
} t_collision;

typedef struct s_triangle_collision {
    float t;

    float u;
    float v;
	t_shape shape;
} t_triangle_collision;

t_collision collide_shape(t_state* state,
                                  t_shape shape,
                                  t_ray ray,
                                  float t_max);


typedef struct t_triangle_pts {
	t_fvec3 a;
	t_fvec3 b;
	t_fvec3 c;
} t_triangle_pts;

t_fvec3 collision_norm(t_state *state, t_collision collision, t_fvec3 pos);
t_bounds3f shape_bounds(t_state *state, t_shape shape);
#endif
