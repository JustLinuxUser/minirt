#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minirt.h"
#include "mymath.h"
#include "samplers.h"

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


int main(void) {
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
                t_sampler_state sstate = {.stratified_x = 2, .stratified_y = 2};

                t_fvec2 sample;
                while (sample_stratified(&sstate, &sample)) {
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
