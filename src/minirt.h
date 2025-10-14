#ifndef MINIRT_H
# define MINIRT_H
#include "MLX42/MLX42.h"
#include "mymath.h"
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
	uint8_t a;
} t_8bcolor;

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

struct s_state ;

typedef struct {
    struct s_state* state;
    int start_pixel;
    int num_pixels;
    int* exit_flag;
} t_render_task;

typedef struct s_renderer_state {
	int curr_px;
    pthread_t threads[100];
    t_render_task tasks[100];
    int exit_flags[100];
	int total_runs;
} t_renderer_state;

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

	t_vec_shape unbounded_shapes;

	t_linear_bvh_nd *bvh;

	// int last_x;
	// int last_y;
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
void createAliasTable(t_lights *lights);
int SampleAliasTable(t_lights *lights, float lu);

// colors.c
t_color RGBToColor(t_color c);
float linear_to_gamma(float c);
/*NEW*/
t_8bcolor ColortoRGB(t_color c);
t_fvec3 SpectrumToXYZ(t_SampledSpectrum s, t_SampledWavelengths lambda);
t_8bcolor XYZToRGB(t_fvec3 t);
uint32_t conv_8bcolor_to_uint32(t_8bcolor c);
t_color spectrum_to_rgb(t_SampledSpectrum s, t_SampledWavelengths lambda);
t_color clamp_rgb(t_color c);

float random_generator();

/*END NEW*/
t_fvec3 perspective_cam_ray(t_state* state, t_fvec2 px, t_fvec2 sample);

// ray.c
t_color cast_reflectable_ray(t_state* state, t_ray ray, int iters_left);
/*TO BE CHANGE ONCE LOGIC CHANGED*/
t_SampledSpectrum cast_reflectable_ray_new(t_state* state, t_ray ray, 
        t_SampledWavelengths lambdas, int iters_left);

// tinyobj.c
void load_triangles(t_state* state, char *path, t_fvec3 pos, float scale);

// draw.c
void loop_hook(void *state_param);

#endif
