#ifndef SHAPES_H
# define SHAPES_H
#include "bounds.h"
#include "ray.h"
#include "mymath.h"
#include <stdbool.h>

#define GEN_TYPE t_fvec3
#define GEN_SHORT_TYPE fvec3
#include "libft/templates/vec_template.hpp"

#define GEN_TYPE int
#include "libft/templates/vec_template.hpp"

struct s_state;

enum obj_type {
    OBJ_SPHERE,
    OBJ_PLANE,
    OBJ_CONE,
    OBJ_CYLINDER,
    OBJ_TRIANGLE,
};

typedef struct s_triangle {
	int mesh_idx;
	int triangle_idx;
}	t_triangle;

#define GEN_TYPE t_triangle
#define GEN_SHORT_TYPE triangle
#include "libft/templates/vec_template.hpp"

typedef struct t_sphere {
    t_fvec3 p;
    float r;
} t_sphere;

#define GEN_TYPE t_sphere
#define GEN_SHORT_TYPE sphere
#include "libft/templates/vec_template.hpp"

typedef struct s_mesh {
	t_vec_fvec3	vertices;
	t_vec_int	vertex_idxs;
} t_mesh;

typedef struct s_plane {
	t_fvec3 pos;
	t_fvec3 dir;
} t_plane;

#define GEN_TYPE t_plane
#define GEN_SHORT_TYPE plane
#include "libft/templates/vec_template.hpp"

#define GEN_TYPE t_mesh
#define GEN_SHORT_TYPE mesh
#include "libft/templates/vec_template.hpp"

typedef struct s_shape {
    enum obj_type type;
	void *ptr;
} t_shape;

#define GEN_TYPE t_shape
#define GEN_SHORT_TYPE shape
#include "libft/templates/vec_template.hpp"

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

t_collision collide_shape(struct s_state* state, t_shape shape, t_ray_isector isector);


typedef struct t_triangle_pts {
	t_fvec3 a;
	t_fvec3 b;
	t_fvec3 c;
} t_triangle_pts;

t_fvec3 collision_norm(struct s_state *state, t_collision collision, t_fvec3 pos);
t_bounds3f shape_bounds(struct s_state *state, t_shape shape);
#endif
