#ifndef MINIRT_H
# define MINIRT_H
#include "mymath.h"
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

typedef struct sphere {
    t_fvec3 p;
    float r;
    t_color color;
} sphere;

enum obj_type {
    OBJ_SPHERE,
    OBJ_PLANE,
};

typedef union obj_u {
    sphere sphere;
    t_ray plane;
} obj_u;

typedef struct obj {
    enum obj_type type;
    obj_u obj;
    bool skip;
} obj;

typedef struct t_state {
    obj objs[1024];
    int obj_count;

    t_color* colors;
    int screen_width;
    int screen_height;

    t_ray cam;
    float cam_pitch;
    float cam_yaw;
    int last_collided_idx;
    int debug;

    t_fvec3 light_pos;
    float proj_coef;
    float screen_dist;
    bool preview;
} t_state;


typedef struct output_config {
    int width;
    int height;
} output_config;

// shapes.c
bool intersect_sphere(sphere s, t_ray r, float* t);
int intersect_plane(t_ray plane, t_ray r, float* t);

// colors.c
t_color RGBToColor(Color c);
float linear_to_gamma(float c);
Color ColortoRGB(t_color c);
t_fvec3 perspective_cam_ray(t_state* state, t_fvec2 px, t_fvec2 sample);

// ray.c
t_color cast_reflectable_ray(t_state* state, t_ray ray, int iters_left);
#endif
