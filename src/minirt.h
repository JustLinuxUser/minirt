#ifndef MINIRT_H
# define MINIRT_H
#include "MLX42/MLX42.h"
#include "MLX42/include/MLX42/MLX42.h"
#include "config.h"
#include "libft/dsa/dyn_str.h"
#include "mymath.h"
#include "obj_loader.h"
#include "spectrum.h"

#include "shapes.h"
#include "bounds.h"
#include "light.h"

#include <pthread.h>
#include <stdbool.h>

typedef t_fvec3 t_color;
typedef struct s_8bcolor {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} t_8bcolor;

#define DARKDARKGRAY (t_color){.x = 0.01, .y = 0.01, .z = 0.01}

typedef struct perspective_cam {
    t_fvec3 pos;
    t_fvec3 dir;
    int res_x;
    int res_y;
    float cam_dist;
} perspective_cam;

typedef struct s_linear_bvh_nd {
    t_bounds3f bounds;
    union {
        int primitives_offset;    // leaf
        int second_child_offset;  // interior
    } _union;
    uint16_t n_primitives;  // 0 -> interior node
    uint8_t axis;           // interior node: xyz
} t_linear_bvh_nd;

struct s_state ;

typedef struct {
    struct s_state* state;
    int start_pixel;
    int num_pixels;
    uint8_t* thrd_state;
	int exit_immediatelly;
	uint64_t prng_state;
} t_render_task;

enum e_THRD_STATE {
	THRD_NONE = 1 << 0,
	THRD_RUNNING = 1 << 1,
	THRD_FINISHED = 1 << 2,
};

typedef struct s_renderer_state {
	int curr_px;
    pthread_t threads[MAX_NUM_THREADS];
    t_render_task tasks[MAX_NUM_THREADS];
    uint8_t thrd_states[MAX_NUM_THREADS];
	int total_runs;
	int num_threads;
	int chunk_size;
	int max_reflections;
	bool render_once;
	bool exit_after_render;

	uint64_t rand_state;
	int rand_px;
} t_renderer_state;

typedef struct s_state {
    t_fvec3* s_colors;

    int screen_width;
    int screen_height;

	int samples_x;
	int samples_y;

    t_ray cam;
	float fov;
    float cam_pitch;
    float cam_yaw;

	t_densely_sampled_spectrum		ambiant_light_spec;
	int		sky_light_idx;

    t_lights lights;

    float proj_coef;
    float screen_dist;

	t_vec_mesh meshes;
	t_vec_triangle triangles;

	t_vec_sphere spheres;
	t_vec_plane planes;
	t_vec_cylinder cylinders;

	t_vec_shape shapes;

	t_vec_shape unbounded_shapes;

	t_vec_densely_sampled_spectrum spectrums;

	t_linear_bvh_nd *bvh;

	t_dyn_str		output_path;
	mlx_t *mlx;
	mlx_image_t *mlx_image;
	t_renderer_state rndr;
} t_state;

t_collision collide_bvh(t_state* state, t_ray_isector isector);

typedef struct output_config {
    int width;
    int height;
} output_config;

// probability.c
void create_alias_table(t_lights *lights);
int sample_alias_table(t_lights *lights, float lu);

// colors.c
float linear_to_gamma(float c);
/*NEW*/
t_8bcolor ColortoRGB(t_color c);
t_fvec3 SpectrumToXYZ(t_SampledSpectrum s, t_SampledWavelengths lambda);
t_8bcolor xyz_to_rgb(t_fvec3 t);
t_fvec3 rgb_to_xyz(t_8bcolor c);
uint32_t conv_8bcolor_to_uint32(t_8bcolor c);
t_fvec3 densely_sampled_spectrum_to_xyz(t_densely_sampled_spectrum s);
t_color clamp_rgb(t_color c);

float rand_float(uint64_t *rand_state);

/*END NEW*/
t_fvec3 perspective_cam_ray(t_state* state, t_fvec2 px, t_fvec2 sample);

// ray.c
t_SampledSpectrum cast_reflectable_ray_new(t_state* state, t_ray ray, 
        t_SampledWavelengths lambdas, int iters_left, uint64_t *rand_state);

// tinyobj.c
bool load_triangles(t_state* state, t_obj_spec s);

// draw.c
void loop_hook(void *state_param);
void exit_hook(void *state_param);

void exit_app(t_state *state);
#endif
