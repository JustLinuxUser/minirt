#ifndef SHAPES_H
# define SHAPES_H
#include "bounds.h"
#include "light.h"
#include "ray.h"
#include "mymath.h"
#include <stdbool.h>

#include "libft/generated/vec_fvec3.h"
#include "libft/generated/vec_int.h"

struct s_state;

enum obj_type {
    OBJ_SPHERE,
    OBJ_PLANE,
    OBJ_CYLINDER,
    OBJ_TRIANGLE,
};

typedef struct s_triangle {
	int mesh_idx;
	int triangle_idx;
}	t_triangle;

#include "libft/generated/vec_triangle.h"

typedef struct t_sphere {
    t_fvec3 p;
    float r;
	int	spectrum_idx;
} t_sphere;

#include "libft/generated/vec_sphere.h"

typedef struct t_cylinder {
	t_fvec3	a;
	t_fvec3	b;
	float	radius;
	int	spectrum_idx;
} t_cylinder;

#include "libft/generated/vec_cylinder.h"


typedef struct s_mesh {
	t_vec_fvec3	vertices;
	t_vec_int	vertex_idxs;
	int	spectrum_idx;
} t_mesh;

typedef struct s_plane {
	t_fvec3 pos;
	t_fvec3 dir;
	int	spectrum_idx;
} t_plane;

#include "libft/generated/vec_plane.h"
#include "libft/generated/vec_mesh.h"

typedef struct s_shape {
    enum obj_type type;
	void *ptr;
} t_shape;

#include "libft/generated/vec_shape.h"

typedef struct s_collision {
    bool collided;
    float t;

    // triangle
    float u;
    float v;

	// cylinder
	t_fvec3 norm;

	t_shape shape;
} t_collision;

typedef struct s_triangle_collision {
    float t;

    float u;
    float v;
	t_shape shape;
} t_triangle_collision;

t_collision collide_shape(struct s_state* state, t_shape shape, t_ray_isector isector);

typedef struct t_triangle_pts {
	t_fvec3 a;
	t_fvec3 b;
	t_fvec3 c;
} t_triangle_pts;

t_fvec3 collision_norm(struct s_state *state, t_collision collision, t_fvec3 pos);
t_bounds3f shape_bounds(struct s_state *state, t_shape shape);
t_densely_sampled_spectrum *shape_spectrum(struct s_state *state, t_collision collision);
#endif
