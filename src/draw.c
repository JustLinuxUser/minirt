#include <pthread.h>
#include "MLX42/MLX42.h"
#include "minirt.h"
#include "samplers.h"
#include "libft/libft.h"

size_t coord_to_idx(t_state* state, int x, int y) {
    return (state->screen_width * y + x);
}

void idx_to_coords(t_state* state, size_t idx, int* x, int* y) {
    *x = idx % state->screen_width;
    *y = idx / state->screen_width;
}

void* render_step(void* arg) {
    // Draw
    t_render_task *t = (t_render_task*)arg;
    t_fvec3 x0y0 = vec3_rotate_yaw_pitch(
        perspective_cam_ray(t->state, (t_fvec2){0}, (t_fvec2){0}),
        t->state->cam_yaw, t->state->cam_pitch);
    t_fvec3 x1y0 = vec3_rotate_yaw_pitch(
        perspective_cam_ray(t->state, (t_fvec2){.x = t->state->screen_width},
                            (t_fvec2){0}),
        t->state->cam_yaw, t->state->cam_pitch);
    t_fvec3 x0y1 = vec3_rotate_yaw_pitch(
        perspective_cam_ray(t->state, (t_fvec2){.y = t->state->screen_height},
                            (t_fvec2){0}),
        t->state->cam_yaw, t->state->cam_pitch);
    t_fvec3 x1y1 = vec3_rotate_yaw_pitch(
        perspective_cam_ray(
            t->state,
            (t_fvec2){.x = t->state->screen_width, .y = t->state->screen_height},
            (t_fvec2){0}),
        t->state->cam_yaw, t->state->cam_pitch);
	fflush(stdout);
    int x, z;
    idx_to_coords(t->state, t->start_pixel, &x, &z);
    while (z < t->state->screen_height) {
        while (x < t->state->screen_width) {
            t_ray curr_ray;
            t_SampledSpectrum res_color = {0};
            t_sampler_state sstate = {.stratified_x = t->state->samples_x, .stratified_y = t->state->samples_y};

            t_fvec2 sample;
            t_fvec3 xyz_color = {0};
            while (sample_stratified(&sstate, &sample)) {
				if (t->exit_immediatelly == 1 || coord_to_idx(t->state, x, z) - t->start_pixel > t->num_pixels - 1) {
					*t->thrd_state = THRD_FINISHED;
					return 0;
				}
                float lu = random_generator();
                t_SampledWavelengths lambdas = SampleUniform(lu, 360, 830);
                curr_ray.pos = t->state->cam.pos;
                t_fvec3 l1 = fvec3_lerp(x0y0, x0y1, (z + sample.y) / t->state->screen_height);
                t_fvec3 l2 = fvec3_lerp(x1y0, x1y1, (z + sample.y) / t->state->screen_height);
                t_fvec3 pt = fvec3_norm(
                    fvec3_lerp(l2, l1, (x + sample.x) / t->state->screen_width));
                curr_ray.dir = pt;

                res_color =
                    cast_reflectable_ray_new(t->state, curr_ray, lambdas, 20);
                xyz_color =
                    fvec3_add(SpectrumToXYZ(res_color, lambdas), xyz_color);
            }
            xyz_color = fvec3_scale(
                xyz_color, 1. / (sstate.stratified_x * sstate.stratified_y));

            // RES COLOR TO XYZ (T_FVEC3)
            t->state->s_colors[z * t->state->screen_width + x] = fvec3_add(
                t->state->s_colors[z * t->state->screen_width + x], xyz_color);
            x++;
        }
        x = 0;
        z++;
    }
    *t->thrd_state = THRD_FINISHED;
    return 0;
}

void drawinator(t_state* state) {
    ft_printf("total_runs: %i\n", state->rndr.total_runs);
    // printf("curr_px: %i\n",
    //        state.last_y * state.screen_width + state.last_x);
	ft_memset(state->mlx_image->pixels, 255, state->mlx_image->width * state->mlx_image->height * sizeof(uint32_t));
	ft_printf("%zu\n", state->rndr.curr_px);
	for (int y = 0; y < state->screen_height; y++) {
		for (int x = 0; x < state->screen_width; x++) {
			mlx_put_pixel(state->mlx_image,
				 x, y,
				 conv_8bcolor_to_uint32(xyz_to_rgb(fvec3_scale(
				 state->s_colors[y * state->screen_width + x],
				 1. / state->rndr.total_runs))));
		}
    }
}

int thread_idx(t_state *state, int mask) {
    for (int j = 0; j < state->rndr.num_threads; j++)
        if (state->rndr.thrd_states[j] & mask)
            return j;
    return -1;
}

int get_num_threads(t_state *state, int mask) {
    int ret = 0;
    for (int j = 0; j < state->rndr.num_threads; j++)
        if (state->rndr.thrd_states[j] & mask)
            ret++;
    return ret;
}

void render_single_thread(t_state* state) {
	double start;
    uint8_t exit_flag;
    int incr = 100;

	start = mlx_get_time();
	while (mlx_get_time() - start < 1.0 / 60) {
		if (state->rndr.curr_px < state->screen_width * state->screen_height) {
			t_render_task render_task = {.state = state,
										 .start_pixel = state->rndr.curr_px,
										 .num_pixels = incr,
										 .thrd_state = &exit_flag};
			render_step(&render_task);
			state->rndr.curr_px += incr;
		} else {
			state->rndr.total_runs++;
			state->rndr.curr_px = 0;
		}
	}
    ft_printf("finished\n");
}

void render_multithread(t_state* state) {
    // render_single_thread(state);
    // return;

    if (state->rndr.curr_px < state->screen_width * state->screen_height) {
		int i = 0;
        int pos;
        while ((pos = thread_idx(state, THRD_NONE | THRD_FINISHED)) != -1
			&& ++i < state->rndr.num_threads) {
            if (state->rndr.thrd_states[pos] == THRD_FINISHED)
                pthread_join(state->rndr.threads[pos], NULL);
            state->rndr.thrd_states[pos] = THRD_RUNNING;
            int incr = 1000;
            state->rndr.tasks[pos] = (t_render_task){.state = state,
                                         .start_pixel = state->rndr.curr_px,
                                         .num_pixels = incr,
                                         .thrd_state = &state->rndr.thrd_states[pos]};
            state->rndr.curr_px += incr;
			ft_printf("creating thread...\n");
            pthread_create(&state->rndr.threads[pos], NULL, render_step, &state->rndr.tasks[pos]);
        }
		ft_printf("exited looop\n");
    } else {
		int curr;
		while ((curr = thread_idx(state, THRD_FINISHED)) != -1) {
			ft_printf("joining thread\n");
			pthread_join(state->rndr.threads[curr], NULL);
			state->rndr.thrd_states[curr] = THRD_NONE;
		}
		if (get_num_threads(state, THRD_NONE) == state->rndr.num_threads) {
			ft_printf("resetting everything\n");
			ft_memset(state->rndr.thrd_states, THRD_NONE, sizeof(state->rndr.thrd_states));
			state->rndr.curr_px = 0;
			state->rndr.total_runs++;
		}
	}
}

void emergency_exit(t_state *state) {
	for (int i = 0; i < state->rndr.num_threads; i++) {
		state->rndr.tasks[i].exit_immediatelly = 1;
	}
	int curr;
	while ((curr = thread_idx(state, THRD_RUNNING | THRD_FINISHED)) != -1) {
		pthread_join(state->rndr.threads[curr], NULL);
		state->rndr.thrd_states[curr] = THRD_NONE;
	}
}
void exit_hook(void *state_arg) {
	t_state *state = (t_state *)state_arg;
	emergency_exit(state);
}

void loop_hook(void* state_param) {
    t_state* state = (t_state*)state_param;
    printf("Loop hook: %lf\n", mlx_get_time());
    // render_single_thread(state);
	render_multithread(state);
    drawinator(state);
}
