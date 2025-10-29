#include <math.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cie.h"
#include "libft/alloc/mmalloc.h"
#include "minirt.h"
#include "mymath.h"
#include "rt_file/rt_consumer.h"
#include "error.h"
#include "libft/libft.h"
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

void	init(t_state *state) {
	ft_memset(state->rndr.thrd_states, THRD_NONE, sizeof(state->rndr.thrd_states));

	// First initialize the things that might fail, so we don't have to free anything really
	//
	// TODO: no resizing, check on target platform
	state->mlx = mlx_init(state->screen_width, state->screen_height, "MiniRT", false);
	if (!state->mlx) critical_error("Failed to initialize mlx");
	state->mlx_image = mlx_new_image(state->mlx, state->screen_width, state->screen_height);
	if (!state->mlx_image) critical_error("Failed to initialize mlx image buffer");
	if (-1 == mlx_image_to_window(state->mlx, state->mlx_image, 0, 0))
		critical_error("Failed to put image to the window");

    state->s_colors = mmalloc(state->screen_width * state->screen_height * sizeof(t_fvec3), "allocating the xyz colors buffer");

    const float screen_dist = 1;

    const float fov_x = state->fov / 180 * PI;
    const float proj_plane_width = 2 * tan(fov_x / 2) * screen_dist;
    const float proj_coef = proj_plane_width / state->screen_width;
    state->proj_coef = proj_coef;
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
    for (size_t i = 0; i < state->cylinders.len; i++) {
        vec_shape_push(
            &state->shapes,
            (t_shape){.type = OBJ_CYLINDER, .ptr = state->cylinders.buff + i});
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
	mlx_terminate(state->mlx);
}

void render_multithread(t_state* state, int num_threads);


int main(int argc, char** argv) {
    t_state state = {
        .screen_width = 800,
        .screen_height = 600,
		.samples_x = 5,
		.samples_y = 5,
		.fov = 90,
        .light_pos = {.y = 0, .z = -100, .x = 0},
        .screen_dist = 1,
		.rndr = {
			.total_runs = 1,
			.num_threads = 8,
		}
    };

    if (argc != 2)
        return (1);

    if (!process_file(argv[1], &state))
	{
        return (free_state(&state), 1);
	}
    init(&state);

	// return (0);
    // if (get_obj(argv[1]))
    //     return 1;
    // return 0;

	state.light_pos = fvec3_sub(state.cam.pos, (t_fvec3){.x = 0});
    load_shapes(&state);
	build_bvh(&state);
// 	printf("min: %f %f %f, max: %f %f %f\n", state.bvh->bounds.min.x, state.bvh->bounds.min.y, state.bvh->bounds.min.z,
// state.bvh->bounds.max.x, state.bvh->bounds.max.y, state.bvh->bounds.max.z
// 		);
// 	return 0;

    ft_printf("triangles: %i\n", (int)state.triangles.len);

    /*LIGHTS*/

    // t_light light1 = {.t = POINT_LIGHT, .intensity = 100.f, .position = {.x = -30, .y = -50}};
    // light1.spec = calculateDenselySampledSpectrum(9000);
    // 
    // t_light light2 = {.t = POINT_LIGHT, .intensity = 1000.f, .position = fvec3_add(state.light_pos, (t_fvec3){.y = 20})};
    // light2.spec = calculateDenselySampledSpectrum(6200);
    //
    // t_light light3 = {.t = POINT_LIGHT, .intensity = 1000.f, .position = fvec3_add(state.light_pos, (t_fvec3){.z = 40, .y = 50})};
    // light3.spec = calculateDenselySampledSpectrum(3000);
    // add_light(&lights, light3);
    //
    // t_light light4 = {.t = POINT_LIGHT, .intensity = 1000.f, .position = fvec3_add(state.light_pos, (t_fvec3){.z = 10, .y = 0})};
    // light4.spec = calculateDenselySampledSpectrum(6000);
    // add_light(&lights, light4);

    // add_light(&lights, light1);
    // add_light(&lights, light2);
    calculatePDFs(&state.lights);
    create_alias_table(&state.lights);


    int num_frames = -10;
    // Main game loop
    state.preview = true;
	// TODO: Close hook
        // Update
	mlx_close_hook(state.mlx, exit_hook, &state);
	mlx_loop_hook(state.mlx, loop_hook, &state);
	mlx_loop(state.mlx);
	free_state(&state);
}
