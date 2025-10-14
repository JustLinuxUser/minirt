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
    t_render_task t = *(t_render_task*)arg;
    t_fvec3 x0y0 = vec3_rotate_pitch_yaw(
        perspective_cam_ray(t.state, (t_fvec2){0}, (t_fvec2){0}),
        t.state->cam_pitch, t.state->cam_yaw);
    t_fvec3 x1y0 = vec3_rotate_pitch_yaw(
        perspective_cam_ray(t.state, (t_fvec2){.x = t.state->screen_width},
                            (t_fvec2){0}),
        t.state->cam_pitch, t.state->cam_yaw);
    t_fvec3 x0y1 = vec3_rotate_pitch_yaw(
        perspective_cam_ray(t.state, (t_fvec2){.y = t.state->screen_height},
                            (t_fvec2){0}),
        t.state->cam_pitch, t.state->cam_yaw);
    t_fvec3 x1y1 = vec3_rotate_pitch_yaw(
        perspective_cam_ray(
            t.state,
            (t_fvec2){.x = t.state->screen_width, .y = t.state->screen_height},
            (t_fvec2){0}),
        t.state->cam_pitch, t.state->cam_yaw);
    int x, y;
    idx_to_coords(t.state, t.start_pixel, &x, &y);
    while (y < t.state->screen_height) {
        while (x < t.state->screen_width) {
            if (coord_to_idx(t.state, x, y) - t.start_pixel >= t.num_pixels) {
                *t.exit_flag = 2;
                return 0;
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
                    x0y0, x0y1, (y + sample.y) / t.state->screen_height);
                t_fvec3 l2 = fvec3_lerp(
                    x1y0, x1y1, (y + sample.y) / t.state->screen_height);
                t_fvec3 pt = fvec3_norm(
                    fvec3_lerp(l2, l1, (x + sample.x) / t.state->screen_width));
                curr_ray.dir = pt;

                res_color =
                    cast_reflectable_ray_new(t.state, curr_ray, lambdas, 2);
                xyz_color =
                    fvec3_add(SpectrumToXYZ(res_color, lambdas), xyz_color);
            }
            xyz_color = fvec3_scale(
                xyz_color, 1. / (sstate.stratified_x * sstate.stratified_y));

            // RES COLOR TO XYZ (T_FVEC3)
            t.state->s_colors[y * t.state->screen_width + x] = fvec3_add(
                t.state->s_colors[y * t.state->screen_width + x], xyz_color);
            x++;
        }
        x = 0;
        y++;
    }
    *t.exit_flag = 2;
    return 0;
}

void drawinator(t_state* state) {
    printf("total_runs: %i\n", state->rndr.total_runs);
    // printf("curr_px: %i\n",
    //        state.last_y * state.screen_width + state.last_x);
	ft_memset(state->mlx_image->pixels, 255, state->mlx_image->width * state->mlx_image->height * sizeof(uint32_t));
	printf("%zu\n", state->rndr.curr_px);
	for (int y = 0; y < state->screen_height; y++) {
		for (int x = 0; x < state->screen_width; x++) {
			mlx_put_pixel(state->mlx_image,
				 x, y,
				 conv_8bcolor_to_uint32(XYZToRGB(fvec3_scale(
				 state->s_colors[y * state->screen_width + x],
				 1. / state->rndr.total_runs))));
            // if (state.total_runs == 1 &&
            //     y * state.screen_width + x >
            //         state.last_y * state.screen_width + state.last_x) {
            //     DrawPixel(x, y, WHITE);
            // } else {
            // TODO: actually draw

            // }
		}
    }
}

int free_thread_idx(int num_threads, int* exit_flags) {
    for (int j = 0; j < num_threads; j++)
        if (exit_flags[j] == 0 || exit_flags[j] == 2)
            return j;
    return -1;
}

int num_free_threads(int num_threads, int* exit_flags) {
    int ret = 0;
    for (int j = 0; j < num_threads; j++)
        if (exit_flags[j] == 0 || exit_flags[j] == 2)
            ret++;
    return ret;
}

void render_single_thread(t_state* state) {
	double start;
    int exit_flag;
    int incr = 100;

	start = mlx_get_time();
	while (mlx_get_time() - start < 1.0 / 60) {
		if (state->rndr.curr_px < state->screen_width * state->screen_height) {
			t_render_task render_task = {.state = state,
										 .start_pixel = state->rndr.curr_px,
										 .num_pixels = incr,
										 .exit_flag = &exit_flag};
			render_step(&render_task);
			state->rndr.curr_px += incr;
		} else {
			state->rndr.total_runs++;
			state->rndr.curr_px = 0;
		}
	}
    printf("finished\n");
}

void render_multithread(t_state* state, int num_threads) {
    // render_single_thread(state);
    // return;

    int i = -1;
    if (state->rndr.curr_px < state->screen_width * state->screen_height) {
        int pos;
        while ((pos = free_thread_idx(num_threads, state->rndr.exit_flags)) != -1) {
            if (state->rndr.exit_flags[pos] == 2)
                pthread_join(state->rndr.threads[pos], NULL);
            state->rndr.exit_flags[pos] = 1;
            int incr = 5000;
            state->rndr.tasks[pos] = (t_render_task){.state = state,
                                         .start_pixel = state->rndr.curr_px,
                                         .num_pixels = incr,
                                         .exit_flag = &state->rndr.exit_flags[pos]};
            state->rndr.curr_px += incr;
            pthread_create(&state->rndr.threads[pos], NULL, render_step, &state->rndr.tasks[pos]);
        }
    } else if (num_free_threads(num_threads, state->rndr.exit_flags) == num_threads) {
		i = -1;
		while (++i < num_threads)
			pthread_join(state->rndr.threads[i], NULL);
		ft_memset(state->rndr.exit_flags, 0, sizeof(state->rndr.exit_flags));
		state->rndr.curr_px = 0;
		state->rndr.total_runs++;
	}
}

void loop_hook(void* state_param) {
    t_state* state = (t_state*)state_param;
    printf("Loop hook: %lf\n", mlx_get_time());
    // render_single_thread(state);
	render_multithread(state, 8);
    drawinator(state);
}
