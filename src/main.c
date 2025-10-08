#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minirt.h"
#include "mymath.h"
#include "obj_file/obj_tokenizer.h"
#include "obj_file/obj_parser.h"
#include "rt_file/rt_consumer.h"
#include "samplers.h"
#include <pthread.h>

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
// - SampledWavelengths are not always 0.5
// - Intel embree shapes collisions
// - Blender cycles shapes collisions
// - Add multithreading ASAP
// - Switch to render once, without the infinite mode
//

t_state init(int screen_width, int screen_height, float fov_deg) {
    t_state ret = {
        .screen_width = screen_width,
        .screen_height = screen_height,
        .light_pos = {.y = 0, .z = -100, .x = 0},
        .screen_dist = 1,
        .total_runs = 0,
        //.debug = 1,
        // .preview = true,
    };

    ret.s_colors = calloc(ret.screen_width * ret.screen_height, sizeof(t_fvec3));

    const float screen_dist = 1;

    const float fov_x = fov_deg / 180 * PI;
    const float proj_plane_width = 2 * tan(fov_x / 2) * screen_dist;
    const float proj_coef = proj_plane_width / ret.screen_width;
    ret.proj_coef = proj_coef;
    return ret;
}

typedef struct {
    t_state *state;
    int num_threads;
    int thread_id;
    int* exit_flag;
} t_render_task;

void* render_step(void *arg) {
    // Draw
    t_render_task t = *(t_render_task*)arg;
    t_fvec3 x0y0 = vec3_rotate_pitch_yaw(
        perspective_cam_ray(t.state, (t_fvec2){}, (t_fvec2){}), t.state->cam_pitch,
        t.state->cam_yaw);
    t_fvec3 x1y0 = vec3_rotate_pitch_yaw(
        perspective_cam_ray(t.state, (t_fvec2){.x = t.state->screen_width},
                            (t_fvec2){}),
        t.state->cam_pitch, t.state->cam_yaw);
    t_fvec3 x0y1 = vec3_rotate_pitch_yaw(
        perspective_cam_ray(t.state, (t_fvec2){.y = t.state->screen_height},
                            (t_fvec2){}),
        t.state->cam_pitch, t.state->cam_yaw);
    t_fvec3 x1y1 = vec3_rotate_pitch_yaw(
        perspective_cam_ray(
            t.state,
            (t_fvec2){.x = t.state->screen_width, .y = t.state->screen_height},
            (t_fvec2){}),
        t.state->cam_pitch, t.state->cam_yaw);
    int x, y = 0;
    while (y < t.state->screen_height) {
        x = 0;
        while (x < t.state->screen_width) {
            if ((y * t.state->screen_width + x) % t.num_threads != t.thread_id)
            {
                x++;
                continue;

            }
            t_ray curr_ray;
            t_SampledSpectrum res_color = {0};
            t_sampler_state sstate = {.stratified_x = 1, .stratified_y = 1};

            t_fvec2 sample;
			t_fvec3 xyz_color = {0};
            while (sample_stratified(&sstate, &sample)) {
				float lu = random_generator();
				t_SampledWavelengths lambdas = SampleUniform(lu, 360, 830);
                curr_ray.pos = t.state->cam.pos;
                t_fvec3 l1 = fvec3_lerp(
                    x0y0, x0y1,
                    (y + sample.y) / t.state->screen_height);
                t_fvec3 l2 = fvec3_lerp(
                    x1y0, x1y1,
                    (y + sample.y) / t.state->screen_height);
                t_fvec3 pt = fvec3_norm(fvec3_lerp(
                    l2, l1, (x + sample.x) / t.state->screen_width));
                curr_ray.dir = pt;

				res_color = cast_reflectable_ray_new(t.state, curr_ray, lambdas, 1);
				xyz_color = fvec3_add(SpectrumToXYZ(res_color, lambdas), xyz_color);
            }
			xyz_color = fvec3_scale(xyz_color, 1. / (sstate.stratified_x *
						   sstate.stratified_y));

			//RES COLOR TO XYZ (T_FVEC3)
			t.state->s_colors[y * t.state->screen_width + x] = fvec3_add(t.state->s_colors[y * t.state->screen_width + x], xyz_color);
            x++;
        }
        y++;
    }
    *t.exit_flag = 1;
}

void drawinator(t_state *state)
{
    BeginDrawing();
    ClearBackground(WHITE);
    printf("total_runs: %i\n", state->total_runs);
    // printf("curr_px: %i\n",
    //        state.last_y * state.screen_width + state.last_x);
    for (int x = 0; x < state->screen_width; x++) {
        for (int y = 0; y < state->screen_height; y++) {
            // if (state.total_runs == 1 &&
            //     y * state.screen_width + x >
            //         state.last_y * state.screen_width + state.last_x) {
            //     DrawPixel(x, y, WHITE);
            // } else {
                DrawPixel(x, y,
                        XYZToRGB(fvec3_scale(state->s_colors[y * state->screen_width + x],
                            1. / state->total_runs))
                );
            // }
        }
    }
    EndDrawing();
}

int threads_finished(int num_threads, int* exit_flags)
{
    int ret = 1;

    for (int j = 0; j < num_threads; j++)
        if (exit_flags[j] == 0)
            ret = 0;
    return ret;
}

void render_multithread(t_state* state, int num_threads){
    pthread_t threads[num_threads];
    t_render_task tasks[num_threads];
    int i = -1;
    int exit_flags[100] = {0};
    state->total_runs++;

    while (++i < num_threads)
    {
        tasks[i] = (t_render_task){ .state = state, .num_threads = num_threads, .thread_id = i, .exit_flag = &exit_flags[i] };
        pthread_create(&threads[i], NULL, render_step, &tasks[i]);
    }
    while (true)
    {
        if (threads_finished(num_threads, exit_flags))
            break;
        drawinator(state);
    }
    i = -1;
    while (++i < num_threads)
        pthread_join(threads[i], NULL);
    
}

void load_shapes(t_state* state) {
    for (size_t i = 0; i < state->triangles.len; i++) {
        vec_shape_push(
            &state->shapes,
            (t_shape){.type = OBJ_TRIANGLE, .ptr = state->triangles.buff + i});
    }
    for (size_t i = 0; i < state->spheres.len; i++) {
        vec_shape_push(
            &state->shapes,
            (t_shape){.type = OBJ_SPHERE, .ptr = state->spheres.buff + i});
    }

	// unbounded shapes
    for (size_t i = 0; i < state->planes.len; i++) {
        vec_shape_push(
            &state->unbounded_shapes,
            (t_shape){.type = OBJ_PLANE, .ptr = state->planes.buff + i});
    }
}
void build_bvh(t_state* state);

void free_state(t_state *state) {
	free(state->unbounded_shapes.buff);
	free(state->shapes.buff);
	free(state->triangles.buff);
	free(state->planes.buff);
	free(state->spheres.buff);
	free(state->s_colors);
}
int main(int argc, char** argv) {
    if (argc != 2)
        return (1);
    t_state state = init(800, 600, 70);

    if (!process_file(argv[1], &state))
	{
        return (free_state(&state), 1);
	}
    // if (get_obj(argv[1]))
    //     return 1;
    // return 0;

	state.light_pos = state.cam.pos;
    // vec_sphere_push(&state.spheres, (t_sphere){
    //                                     .r = 150,
    //                                     .p = {.x = 0, .y = 0, .z = 75},
    //                                 });

    // vec_sphere_push(&state.spheres, (t_sphere){
    //                                     .r = 3,
    //                                     .p = {.x = 0, .y = 40, .z = 75},
    //                                 });
    //
    // vec_sphere_push(&state.spheres, (t_sphere){
    //                                     .r = 3,
    //                                     .p = {.x = -12, .y = 30, .z = 75},
    //                                 });
    //
    // vec_sphere_push(&state.spheres, (t_sphere){
    //                                     .r = 3,
    //                                     .p = {.x = -30, .y = -29, .z = 75},
    //                                 });
    // //
    // vec_sphere_push(&state.spheres, (t_sphere){
    //                                     .r = 3,
    //                                     .p = {.x = -2, .y = 23, .z = 75},
    //                                 });
    // vec_sphere_push(&state.spheres, (t_sphere){
    //                                     .r = 3,
    //                                     .p = {.x = 20, .y = 10, .z = 75},
    //                                 });
    load_shapes(&state);
	build_bvh(&state);
// 	printf("min: %f %f %f, max: %f %f %f\n", state.bvh->bounds.min.x, state.bvh->bounds.min.y, state.bvh->bounds.min.z,
// state.bvh->bounds.max.x, state.bvh->bounds.max.y, state.bvh->bounds.max.z
// 		);
// 	return 0;

    printf("triangles: %zu\n", state.triangles.len);

    /*LIGHTS*/
    t_lights lights = {0};

    t_light light1 = {.t = POINT_LIGHT, .intensity = 8000.f, .position = state.light_pos};
    light1.spec = calculateDenselySampledSpectrum(9000);
    
    t_light light2 = {.t = POINT_LIGHT, .intensity = 10000.f, .position = {.y = 70, .z = 150, .x = 100}};
    light2.spec = calculateDenselySampledSpectrum(6200);

    t_light light3 = {.t = POINT_LIGHT, .intensity = 15000.f, .position = {.y = 70, .z = 50, .x = 50}};
    light2.spec = calculateDenselySampledSpectrum(3000);

    add_light(&lights, light1);
    // add_light(&lights, light2);
    // add_light(&lights, light3);
    calculatePDFs(&lights);
    createAliasTable(&lights);

    state.lights = lights;

    InitWindow(state.screen_width, state.screen_height,
               "raylib [core] example - basic window");

    SetTargetFPS(
        60);  // Set our game to run at 60 frames-per-second (Yea! no way!)

    int num_frames = -10;
    // Main game loop
    state.preview = true;
    while (!WindowShouldClose() &&
           num_frames--)  // Detect window close button or ESC key
    {
        int reset = 0;
        if (IsKeyDown(KEY_Q)) {
            state.cam.pos.y += 10;
            reset = 1;
        }
        if (IsKeyPressed(KEY_P)) {
            state.preview ^= true;
            printf("preview: %i\n", state.preview);
            reset = 1;
        }
        if (IsKeyDown(KEY_E)) {
            state.cam.pos.y -= 10;
            reset = 1;
        }
        if (IsKeyDown(KEY_W)) {
            state.cam.pos = fvec3_add(
                state.cam.pos,
                vec3_rotate_pitch_yaw((t_fvec3){.z = 1}, 0, state.cam_yaw));
            reset = 1;
        }
        if (IsKeyDown(KEY_S)) {
            state.cam.pos = fvec3_add(
                state.cam.pos, vec3_rotate_pitch_yaw((t_fvec3){.z = 1}, 0,
                                                     PI + state.cam_yaw));
            reset = 1;
        }
        if (IsKeyDown(KEY_A)) {
            state.cam.pos = fvec3_add(
                state.cam.pos, vec3_rotate_pitch_yaw((t_fvec3){.z = 1}, 0,
                                                     -PI / 2 + state.cam_yaw));
            reset = 1;
        }
        if (IsKeyDown(KEY_D)) {
            state.cam.pos = fvec3_add(
                state.cam.pos, vec3_rotate_pitch_yaw((t_fvec3){.z = 1}, 0,
                                                     PI / 2 + state.cam_yaw));
            reset = 1;
        }

        if (IsKeyDown(KEY_RIGHT)) {
            state.cam_yaw += 0.1;
            reset = 1;
        }
        if (IsKeyDown(KEY_LEFT)) {
            state.cam_yaw -= 0.1;
            reset = 1;
        }
        if (IsKeyDown(KEY_UP)) {
            state.cam_pitch -= 0.1;
            reset = 1;
        }
        if (IsKeyDown(KEY_DOWN)) {
            state.cam_pitch += 0.1;
            reset = 1;
        }

        if (reset) {
            printf("reset\n");
            memset(state.s_colors, 0,
                   state.screen_width * state.screen_height *
                       sizeof(t_SampledSpectrum));
            state.total_runs = 1;
        }
        // Update
        double start = GetTime();
        render_multithread(&state, 8);
        double end = GetTime();
        printf("%lf\n", end - start);

        //----------------------------------------------------------------------------------
    }
}
