#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymath.h"

// TODOS:
// - Generics
// 		- don't need em
// - BRDF
// 		- Not that good, very difficult to undesrtand (Understanding the
// Masking-Shadowing Function in Microfacet-Based BRDFs)
// https://jcgt.org/published/0003/02/03/paper.pdf
// 		- https://www.youtube.com/watch?v=FUZJNlRqrAc
// 		- https://www.pbr-book.org/4ed/contents
//
// - Bump maps
// - Optimize the intersection functions
// - Fix the camera rotations
// - Parsers
// - Cast ray function
//    - With a fast mode, that just checks for intersections
// - Env map (HDRi / equivalent)
// - multithreadeing (compile time configurable)
// - bounding volume hierarchies
// - Linear colors
//
// https://stackoverflow.com/questions/10612262/dealing-with-point-lights-and-exact-reflections-in-path-tracing
//
//

typedef t_fvec3 t_color;
#define DARKDARKGRAY (t_color){.x = 0.01, .y = 0.01, .z = 0.01}

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

bool intersect_sphere(sphere s, t_ray r, float* t) {
    t_fvec3 L = fvec3_sub(s.p, r.pos);

    // if tc is less then 0, we are looking away from the center (mb we are
    // inside the sphere)
    float tc = fvec3_dot(L, fvec3_norm(r.dir));
    // printf("tc: %f\n", tc);
    // if (tc < 0.0) {
    //     return false;
    // }

    // TODO: Optimize the sqrt
    float d = fvec3_dot(L, L) - tc * tc;
    if (d > s.r * s.r || d < 0) {
        return (false);
    }
    float tlc = sqrt(s.r * s.r - d);
    float t1 = tc + tlc;
    float t2 = tc - tlc;

    if ((t1 < t2 && t1 > 0) || t2 < 0)
        *t = t1;
    else
        *t = t2;
    // *p = fvec3_add(Vector3Scale(r.direction, closest_t), r.position);
    // return (true);
    *t *= 0.99;
    return (*t > 0.1);
}

int intersect_plane(t_ray plane, t_ray r, float* t) {
    float denom = fvec3_dot(r.dir, plane.dir);
    // close enough to parallel, to be considered paralell
    if (fabs(denom) < 1e-7) {
        return false;
    }
    *t = fvec3_dot(fvec3_sub(plane.pos, r.pos), plane.dir) / denom;
    if (*t < 1e-6)
        return false;
    // *p = fvec3_add(Vector3Scale(r.direction, t), r.position);
    return (true);
}

t_fvec3 vec3_rotate_pitch_yaw(t_fvec3 v, float pitch, float yaw) {
    Vector3 ret = Vector3RotateByQuaternion(*(Vector3*)(void*)&v,
                                            QuaternionFromEuler(pitch, yaw, 0));
    return (*(t_fvec3*)(void*)&ret);
}

#define WAVELENGHTHS 3;

typedef struct output_config {
    int width;
    int height;
} output_config;

typedef struct sampler_state {
    int stratified_x;
    int stratified_y;

    int last_x;
    int last_y;
} t_sampler_state;

bool sample_stratified(t_sampler_state* state, t_fvec2* sample) {
    sample->x = (float)state->last_x / state->stratified_x;
    sample->y = (float)state->last_y / state->stratified_y;
    state->last_x++;
    if (state->last_x == state->stratified_x) {
        state->last_x = 0;
        state->last_y++;
    }
    if (state->last_y == state->stratified_y) {
        return false;
    }
    return true;
}

typedef struct perspective_cam {
    t_fvec3 pos;
    t_fvec3 dir;
    int res_x;
    int res_y;
    float cam_dist;
} perspective_cam;

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

t_fvec3 perspective_cam_ray(t_state* state, t_fvec2 px, t_fvec2 sample) {
    t_fvec3 dir = {
        .x = (px.x + sample.x - state->screen_width / 2.) * state->proj_coef,
        .y = (px.y + sample.y - state->screen_height / 2.) * state->proj_coef,
        .z = state->screen_dist};
    return fvec3_norm(dir);
}

t_state init(int screen_width, int screen_height, float fov_deg) {
    t_state ret = {
        .screen_width = screen_width,
        .screen_height = screen_height,
        .light_pos = {.y = 40, .z = 150, .x = -100},
        .screen_dist = 1,
        // .preview = true,
    };

    ret.colors = calloc(ret.screen_width * ret.screen_height, sizeof(t_color));

    const float screen_dist = 1;

    const float fov_x = fov_deg / 180 * PI;
    const float proj_plane_width = 2 * tan(fov_x / 2) * screen_dist;
    const float proj_coef = proj_plane_width / ret.screen_width;
    ret.proj_coef = proj_coef;
    return ret;
}

void add_obj(t_state* state, obj o) {
    state->objs[state->obj_count++] = o;
}

float cast_ray(t_state* state, t_ray curr_ray, float t_cutoff) {
    float t;

    int curr_collided_idx = -1;
    float curr_t = INFINITY;

    for (int obj_idx = 0; obj_idx < state->obj_count; obj_idx++) {
        obj o = state->objs[obj_idx];

        if (/* obj_idx == state->last_collided_idx || */ (t_cutoff && o.skip))
            continue;

        if (o.type == OBJ_SPHERE &&
            intersect_sphere(o.obj.sphere, curr_ray, &t)) {
            if (t < curr_t) {
                curr_t = t;
                curr_collided_idx = obj_idx;
            }
        }
        if (o.type == OBJ_PLANE && intersect_plane(o.obj.plane, curr_ray, &t)) {
            if (t < curr_t) {
                curr_t = t * 0.99;
                curr_collided_idx = obj_idx;
            }
        }
        if (t_cutoff != 0 && curr_t < t_cutoff)
            break;
    }
    if (t_cutoff != 0) {
        if (curr_t < t_cutoff) {
            return (0);
        } else {
            return (curr_t);
        }
    } else if (curr_collided_idx != -1) {
        state->last_collided_idx = curr_collided_idx;
        return (curr_t);
    }
    return (0);
}

t_color RGBToColor(Color c) {
    return (t_color){.x = c.r / 255.0, .y = c.g / 255.0, .z = c.b / 255.0};
}

float linear_to_gamma(float c) {
    if (c < 0)
        return 0;
    return (sqrt(c));
}
Color ColortoRGB(t_color c) {
    return (Color){.a = 255,
                   .r = fmin(linear_to_gamma(c.x) * 255, 255),
                   .g = fmin(linear_to_gamma(c.y) * 255, 255),
                   .b = fmin(linear_to_gamma(c.z) * 255, 255)};
}

t_fvec3 rand_direction() {
    t_fvec3 ret;

    while (1) {
        ret.x = (rand() / (float)RAND_MAX) * 2. - 1.;
        ret.y = (rand() / (float)RAND_MAX) * 2. - 1.;
        ret.z = (rand() / (float)RAND_MAX) * 2. - 1.;
        // TODO: If rand is close to 0 we are fucked!!
        // Make this value as close to 0 as possible
        float lensq = fvec3_len_sq(ret);
        if (lensq > 0.00000001 && lensq < 1)
            break;
    }
    return (fvec3_norm(ret));
}

t_fvec3 rand_halfsphere(t_fvec3 norm) {
    t_fvec3 ret = rand_direction();

    if (fvec3_dot(ret, norm) < 0) {
        ret = fvec3_invert(ret);
    }
    return (ret);
}

t_color cast_reflectable_ray(t_state* state, t_ray ray, int iters_left) {
    float t;
    t_color ret = {};
    t_color obj_color;
    t_fvec3 p;

    t = cast_ray(state, ray, 0);

    if (t != 0) {
        obj o = state->objs[state->last_collided_idx];
        t_fvec3 norm;

        if (o.type == OBJ_SPHERE) {
            p = fvec3_add(fvec3_scale(ray.dir, t), ray.pos);
            if ((abs((int)p.x / 4) % 2 == 0) ^ (p.x > 0) ^ (p.z > 0) ^
                (abs((int)p.z / 4) % 2 == 0))
                obj_color = o.obj.sphere.color;
            else
                obj_color = DARKDARKGRAY;
            norm = fvec3_norm(fvec3_sub(p, o.obj.sphere.p));
        }
        if (o.type == OBJ_PLANE) {
            p = fvec3_add(fvec3_scale(ray.dir, t), ray.pos);
            if ((abs((int)p.x / 10) % 2 == 0) ^ (p.x > 0) ^ (p.z > 0) ^
                (abs((int)p.z / 10) % 2 == 0))
                obj_color = DARKDARKGRAY;
            else
                obj_color = RGBToColor(WHITE);
            norm = o.obj.plane.dir;
        }
        t_fvec3 light = fvec3_sub(state->light_pos, p);
        float light_t = sqrtf(fvec3_len_sq(light));
        t_fvec3 norm_light = fvec3_norm(light);
        // cast_ray(&state,
        // curr_color =
        float brightness = 0;

        state->debug = 0;
        float got_t =
            cast_ray(state, (t_ray){.pos = p, .dir = norm_light}, light_t);
        if (got_t) {
            float dot = fmax(fvec3_dot(norm, norm_light), 0);
            brightness = ((15000 * dot) / (light_t * light_t));
        }
        ret = fvec3_add(fvec3_scale(obj_color, brightness), ret);
        if (iters_left > 0) {
            t_ray new_ray = (t_ray){.pos = p, .dir = rand_halfsphere(norm)};
            if (state->last_collided_idx == 1) {
                new_ray.dir = fvec3_norm(fvec3_reflect(ray.dir, norm));
                // return (t_color){.x=1};
            }
            t_color ray_color =
                cast_reflectable_ray(state, new_ray, iters_left - 1);

            t_color ray_color_scaled =
                fvec3_scale(ray_color, fmax(fvec3_dot(new_ray.dir, norm), 0));
            ret = fvec3_add(fvec3_mul(ray_color_scaled, obj_color), ret);
        }
        // ret = Ve
        // if (brightness == 0)
        // 	debug = 1;
        // ret = ;
        return (ret);
    } else {
        return (t_color){};
    }
}

int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------

    t_state state = init(800, 600, 70);

    add_obj(&state, (obj){.type = OBJ_SPHERE,
                          .obj = {.sphere = {.r = 10,
                                             .p = {.x = 0, .y = 0, .z = 100},
                                             .color = RGBToColor(GREEN)}}});
    add_obj(&state, (obj){.type = OBJ_SPHERE,
                          .obj = {.sphere = {.r = 30,
                                             .p = {.x = 20, .y = 20, .z = 150},
                                             .color = RGBToColor(BLUE)}}});
    add_obj(&state, (obj){.type = OBJ_SPHERE,
                          .obj = {.sphere = {.r = 5,
                                             .p = {.x = 40, .y = 30, .z = 100},
                                             .color = RGBToColor(PURPLE)}}});
    add_obj(&state, (obj){.type = OBJ_SPHERE,
                          .obj = {.sphere = {.r = 20,
                                             .p = {.x = 30, .y = -25, .z = 120},
                                             .color = RGBToColor(GREEN)}}});
    add_obj(&state, (obj){.type = OBJ_PLANE,
                          .obj = {.plane = {.pos = {.x = 0, .y = -50, .z = 0},
                                            .dir = fvec3_norm((t_fvec3){
                                                .x = 0, .y = 1, .z = 0})}}});
    add_obj(&state, (obj){.type = OBJ_PLANE,
                          .obj = {.plane = {.pos = {.x = 0, .y = 0, .z = 400},
                                            .dir = fvec3_norm((t_fvec3){
                                                .x = 0, .y = 0, .z = -1})}}});

    add_obj(&state, (obj){.type = OBJ_PLANE,
                          .obj = {.plane = {.pos = {.x = 0, .y = 150, .z = 0},
                                            .dir = fvec3_norm((t_fvec3){
                                                .x = 0, .y = -1, .z = 0})}}});

    add_obj(&state, (obj){.type = OBJ_PLANE,
                          .obj = {.plane = {.pos = {.x = 150, .y = 0, .z = 0},
                                            .dir = fvec3_norm((t_fvec3){
                                                .x = -1, .y = 0, .z = 0})}}});

    add_obj(&state, (obj){.type = OBJ_PLANE,
                          .obj = {.plane = {.pos = {.x = 0, .y = 0, .z = -10},
                                            .dir = fvec3_norm((t_fvec3){
                                                .x = 0, .y = 0, .z = 1})}}});
    add_obj(&state, (obj){.type = OBJ_PLANE,
                          .obj = {.plane = {.pos = {.x = -150, .y = 0, .z = 0},
                                            .dir = fvec3_norm((t_fvec3){
                                                .x = 1, .y = 0, .z = 0})}}});

    add_obj(&state, (obj){.type = OBJ_SPHERE,
                          .skip = true,
                          .obj = {.sphere = {.r = 5,
                                             .p = state.light_pos,
                                             .color = RGBToColor(YELLOW)}}});

    InitWindow(state.screen_width, state.screen_height,
               "raylib [core] example - basic window");

    SetTargetFPS(
        60);  // Set our game to run at 60 frames-per-second (Yea! no way!)

    int num_frames = -10;
    int total_runs = 0;
    // Main game loop
    while (!WindowShouldClose() &&
           num_frames--)  // Detect window close button or ESC key
    {
        int reset = 0;
        if (IsKeyPressed(KEY_Q)) {
            state.cam.pos.y -= 10;
            reset = 1;
        }
        if (IsKeyPressed(KEY_P)) {
            state.preview ^= true;
            printf("preview: %i\n", state.preview);
            reset = 1;
        }
        if (IsKeyPressed(KEY_E)) {
            state.cam.pos.y += 10;
            reset = 1;
        }
        if (IsKeyPressed(KEY_W)) {
            state.cam.pos = fvec3_add(
                state.cam.pos,
                vec3_rotate_pitch_yaw((t_fvec3){.z = 1}, 0, state.cam_yaw));
            reset = 1;
        }
        if (IsKeyPressed(KEY_S)) {
            state.cam.pos = fvec3_add(
                state.cam.pos, vec3_rotate_pitch_yaw((t_fvec3){.z = 1}, 0,
                                                     PI + state.cam_yaw));
            reset = 1;
        }
        if (IsKeyPressed(KEY_A)) {
            state.cam.pos = fvec3_add(
                state.cam.pos, vec3_rotate_pitch_yaw((t_fvec3){.z = 1}, 0,
                                                     -PI / 2 + state.cam_yaw));
            reset = 1;
        }
        if (IsKeyPressed(KEY_D)) {
            state.cam.pos = fvec3_add(
                state.cam.pos, vec3_rotate_pitch_yaw((t_fvec3){.z = 1}, 0,
                                                     PI / 2 + state.cam_yaw));
            reset = 1;
        }

        if (IsKeyPressed(KEY_RIGHT)) {
            state.cam_yaw += 0.1;
            reset = 1;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            state.cam_yaw -= 0.1;
            reset = 1;
        }
        if (IsKeyPressed(KEY_UP)) {
            state.cam_pitch += 0.1;
            reset = 1;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            state.cam_pitch -= 0.1;
            reset = 1;
        }

        if (reset) {
            memset(state.colors, 0,
                   state.screen_width * state.screen_height * sizeof(t_color));
            total_runs = 0;
        }
        // Update

        // Draw

        double start = GetTime();
        ClearBackground(WHITE);
        total_runs += 1;
        BeginDrawing();
        t_fvec3 x0y0 = perspective_cam_ray(&state, (t_fvec2){}, (t_fvec2){});
        t_fvec3 x1y0 = perspective_cam_ray(
            &state, (t_fvec2){.x = state.screen_width}, (t_fvec2){});
        t_fvec3 x0y1 = perspective_cam_ray(
            &state, (t_fvec2){.y = state.screen_height}, (t_fvec2){});
        t_fvec3 x1y1 = perspective_cam_ray(
            &state,
            (t_fvec2){.x = state.screen_width, .y = state.screen_height},
            (t_fvec2){});

        for (int x = 0; x < state.screen_width; x++) {
            for (int y = 0; y < state.screen_height; y++) {
                int done = x * state.screen_height + y;
                t_ray curr_ray;
                t_color res_color = {};
                t_sampler_state sstate = {.stratified_x = 4, .stratified_y = 4};

                t_fvec2 sample;
                while (sample_stratified(&sstate, &sample)) {
                    // curr_ray.dir = perspective_cam_ray(
                    //     &state, (t_fvec2){.x = x, .y = y}, sample);
                    curr_ray.pos = state.cam.pos;
                    t_fvec3 l1 = fvec3_lerp(
                        x0y0, x0y1,  (y + sample.y) / state.screen_height);
                    t_fvec3 l2 = fvec3_lerp(
                        x1y0, x1y1, (y + sample.y) / state.screen_height);
                    t_fvec3 pt = fvec3_norm(fvec3_lerp(
                        l2, l1,  (x + sample.x)/ state.screen_width));
					curr_ray.dir = pt;

                    if (!state.preview)
                        res_color =
                            fvec3_add(cast_reflectable_ray(&state, curr_ray, 6),
                                      res_color);
                    else
                        res_color =
                            fvec3_add(cast_reflectable_ray(&state, curr_ray, 0),
                                      res_color);
                }

                res_color = fvec3_scale(res_color, 1. / (sstate.stratified_x *
                                                         sstate.stratified_y));
                if (state.debug)
                    res_color = RGBToColor(RED);
                state.colors[y * state.screen_width + x] = fvec3_add(
                    state.colors[y * state.screen_width + x], res_color);
            }
        }
        for (int x = 0; x < state.screen_width; x++) {
            for (int y = 0; y < state.screen_height; y++) {
                DrawPixel(x, y,
                          ColortoRGB(fvec3_scale(
                              state.colors[y * state.screen_width + x],
                              1. / total_runs)));
            }
        }
        EndDrawing();
        double end = GetTime();
        printf("%lf\n", end - start);

        //----------------------------------------------------------------------------------
    }
}
