#include <math.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minirt.h"
#include "mymath.h"
#include "rt_file/rt_consumer.h"
#include "samplers.h"
#include "MLX42/MLX42.h"
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
		.rndr = {
			.total_runs = 1,
		}
    };

    ret.s_colors = calloc(ret.screen_width * ret.screen_height, sizeof(t_fvec3));

    const float screen_dist = 1;

    const float fov_x = fov_deg / 180 * PI;
    const float proj_plane_width = 2 * tan(fov_x / 2) * screen_dist;
    const float proj_coef = proj_plane_width / ret.screen_width;
    ret.proj_coef = proj_coef;

	ret.mlx = mlx_init(screen_width, screen_height, "MiniRT", false);
	ret.mlx_image = mlx_new_image(ret.mlx, screen_width, screen_height);
	mlx_image_to_window(ret.mlx, ret.mlx_image, 0, 0);
    return ret;
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

void render_multithread(t_state* state, int num_threads);

int main(int argc, char** argv) {
    if (argc != 2)
        return (1);
    t_state state = init(1000, 600, 70);

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

    t_light light1 = {.t = POINT_LIGHT, .intensity = 800.f, .position = state.light_pos};
    light1.spec = calculateDenselySampledSpectrum(9000);
    
    t_light light2 = {.t = POINT_LIGHT, .intensity = 1000.f, .position = {.y = 70, .z = 150, .x = 100}};
    light2.spec = calculateDenselySampledSpectrum(6200);

    t_light light3 = {.t = POINT_LIGHT, .intensity = 1500.f, .position = {.y = 70, .z = 50, .x = 50}};
    light2.spec = calculateDenselySampledSpectrum(3000);

    add_light(&lights, light1);
    // add_light(&lights, light2);
    // add_light(&lights, light3);
    calculatePDFs(&lights);
    createAliasTable(&lights);

    state.lights = lights;

    int num_frames = -10;
    // Main game loop
    state.preview = true;
	// TODO: Close hook
        // Update
	mlx_loop_hook(state.mlx, loop_hook, &state);
	mlx_loop(state.mlx);
}
