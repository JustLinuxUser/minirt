#ifndef MINIRT_H
# define MINIRT_H
#include "mymath.h"
#include "spectrum.h"

#include "vecs/vec_mesh.h"
#include "shapes.h"
#include "vecs/vec_shape.h"
#include "vecs/vec_sphere.h"
#include "vecs/vec_plane.h"
#include "vecs/vec_triangle.h"
#include "light.h"
#include "obj_file/obj_tokenizer.h"

#include <raylib.h>
#include <stdbool.h>

typedef t_fvec3 t_color;
#define DARKDARKGRAY (t_color){.x = 0.01, .y = 0.01, .z = 0.01}

typedef struct perspective_cam {
    t_fvec3 pos;
    t_fvec3 dir;
    int res_x;
    int res_y;
    float cam_dist;
} perspective_cam;

// TODO: Set allignment to 32
typedef struct s_linear_bvh_nd {
    t_bounds3f bounds;
    union {
        int primitives_offset;    // leaf
        int second_child_offset;  // interior
    } _union;
    uint16_t n_primitives;  // 0 -> interior node
    uint8_t axis;           // interior node: xyz
} t_linear_bvh_nd;

typedef struct s_state {
    t_fvec3* s_colors;

    int screen_width;
    int screen_height;

    t_ray cam;
    float cam_pitch;
    float cam_yaw;
    int last_collided_idx;
    int debug;

    t_fvec3 light_pos;
    t_light light;
    t_lights lights;
    float proj_coef;
    float screen_dist;
    bool preview;

	t_vec_mesh meshes;
	t_vec_triangle triangles;

	t_vec_sphere spheres;
	t_vec_plane planes;

	t_vec_shape shapes;

	t_linear_bvh_nd *bvh;

	// int last_x;
	// int last_y;
	int total_runs;
} t_state;

t_collision collide_bvh(t_state* state, t_ray ray, float t_max, int *steps);


typedef struct output_config {
    int width;
    int height;
} output_config;

// probability.c
void createAliasTable(t_lights *lights);
int SampleAliasTable(t_lights *lights, float lu);

// colors.c
t_color RGBToColor(Color c);
float linear_to_gamma(float c);
/*NEW*/
Color ColortoRGB(t_color c);
t_fvec3 SpectrumToXYZ(t_SampledSpectrum s, t_SampledWavelengths lambda);
Color XYZToRGB(t_fvec3 t);
t_ColorRGB spectrum_to_rgb(t_SampledSpectrum s, t_SampledWavelengths lambda);
t_ColorRGB clamp_rgb(t_ColorRGB c);
float random_generator();
/*END NEW*/
t_fvec3 perspective_cam_ray(t_state* state, t_fvec2 px, t_fvec2 sample);

// ray.c
t_color cast_reflectable_ray(t_state* state, t_ray ray, int iters_left);
/*TO BE CHANGE ONCE LOGIC CHANGED*/
t_SampledSpectrum cast_reflectable_ray_new(t_state* state, t_ray ray, 
        t_SampledWavelengths lambdas, int iters_left);

// tinyobj.c
void load_triangles(t_state* state);
#endif
