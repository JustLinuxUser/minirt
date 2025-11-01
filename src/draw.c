#include <pthread.h>
#include <threads.h>
#include <unistd.h>
#include "MLX42/MLX42.h"
#include "libft/libft.h"
#include "minirt.h"
#include "rt_file/rt_parser.h"
#include "samplers.h"

uint64_t move_tl_prng(t_state* state) {
    while (state->rndr.rand_px != state->rndr.curr_px) {
        state->rndr.rand_px++;
        state->rndr.rand_px =
            state->rndr.rand_px % (state->screen_width * state->screen_height);
        xorshift64(&state->rndr.rand_state);
    }
    return (state->rndr.rand_state);
}

int coord_to_idx(t_state* state, int x, int y) {
    return (state->screen_width * y + x);
}

void idx_to_coords(t_state* state, size_t idx, int* x, int* y) {
    *x = idx % state->screen_width;
    *y = idx / state->screen_width;
}

void write_image_to_ppm(t_state* state) {
    t_dyn_str buff = {0};
    dyn_str_pushstr(&buff, "P3\n");
    dyn_str_pushnbr(&buff, state->screen_width);
    dyn_str_pushstr(&buff, " ");
    dyn_str_pushnbr(&buff, state->screen_height);
    dyn_str_pushstr(&buff, "\n");
    dyn_str_pushnbr(&buff, 255);
    dyn_str_pushstr(&buff, "\n");

    for (int y = 0; y < state->screen_height; y++) {
        for (int x = 0; x < state->screen_width; x++) {
            uint32_t rgba =
                ((uint32_t*)
                     state->mlx_image->pixels)[y * state->screen_width + x];
            uint8_t* p = (uint8_t*)&rgba;

            dyn_str_pushnbr(&buff, p[0]);
            dyn_str_pushstr(&buff, " ");
            dyn_str_pushnbr(&buff, p[1]);
            dyn_str_pushstr(&buff, " ");
            dyn_str_pushnbr(&buff, p[2]);
            dyn_str_pushstr(&buff, " ");
        }
        dyn_str_pushstr(&buff, "\n");
    }
    dyn_str_write_file(buff, state->output_path.buff);
    free(buff.buff);
}

void* render_step(void* arg) {
    // Draw
    t_render_task* t = (t_render_task*)arg;
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
        perspective_cam_ray(t->state,
                            (t_fvec2){.x = t->state->screen_width,
                                      .y = t->state->screen_height},
                            (t_fvec2){0}),
        t->state->cam_yaw, t->state->cam_pitch);
    fflush(stdout);
    int x, z;
    idx_to_coords(t->state, t->start_pixel, &x, &z);
    while (z < t->state->screen_height) {
        while (x < t->state->screen_width) {
            t_ray curr_ray;
            t_SampledSpectrum res_color = {0};
            t_sampler_state sstate = {.stratified_x = t->state->samples_x,
                                      .stratified_y = t->state->samples_y};

            t_fvec2 sample;
            t_fvec3 xyz_color = {0};

            uint64_t rand_state = xorshift64(&t->prng_state);
            // rand_state = (z * t->state->screen_width + x) *
            // t->state->rndr.total_runs; rand_float(&rand_state);
            rand_state++;
            while (sample_stratified(&sstate, &sample)) {
                if (t->exit_immediatelly == 1 ||
                    coord_to_idx(t->state, x, z) - t->start_pixel >
                        t->num_pixels - 1) {
                    *t->thrd_state = THRD_FINISHED;
                    return 0;
                }
                curr_ray.pos = t->state->cam.pos;
                t_fvec3 l1 = fvec3_lerp(
                    x0y0, x0y1, (z + sample.y) / t->state->screen_height);
                t_fvec3 l2 = fvec3_lerp(
                    x1y0, x1y1, (z + sample.y) / t->state->screen_height);
                t_fvec3 pt = fvec3_norm(fvec3_lerp(
                    l2, l1, (x + sample.x) / t->state->screen_width));
                curr_ray.dir = pt;

                float lu = rand_float(&rand_state);
                t_SampledWavelengths lambdas = SampleUniform(lu, 360, 830);

                res_color = cast_reflectable_ray_new(
                    t->state, curr_ray, lambdas, t->state->rndr.max_reflections,
                    &rand_state);
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

void draw(t_state* state) {
    ft_printf("total_runs: %i\n", state->rndr.total_runs);
    // printf("curr_px: %i\n",
    //        state.last_y * state.screen_width + state.last_x);
    ft_memset(
        state->mlx_image->pixels, 255,
        state->mlx_image->width * state->mlx_image->height * sizeof(uint32_t));
    ft_printf("%i\n", state->rndr.curr_px);
    for (int y = 0; y < state->screen_height; y++) {
        for (int x = 0; x < state->screen_width; x++) {
            int total_runs_px = state->rndr.total_runs;
            if (coord_to_idx(state, x, y) <= state->rndr.curr_px) {
                total_runs_px++;
            }
            mlx_put_pixel(state->mlx_image, x, y,
                          conv_8bcolor_to_uint32(xyz_to_rgb(fvec3_scale(
                              state->s_colors[y * state->screen_width + x],
                              1. / total_runs_px))));
        }
    }
}

int thread_idx(t_state* state, int mask) {
    for (int j = 0; j < state->rndr.num_threads; j++)
        if (state->rndr.thrd_states[j] & mask)
            return j;
    return -1;
}

int get_num_threads(t_state* state, int mask) {
    int ret = 0;
    for (int j = 0; j < state->rndr.num_threads; j++)
        if (state->rndr.thrd_states[j] & mask)
            ret++;
    return ret;
}

void render_single_thread(t_state* state) {
    double start;
    uint8_t exit_flag;

    if (state->rndr.curr_px == 0 && state->rndr.render_once &&
        state->rndr.total_runs > 0)
        return;
    start = mlx_get_time();
    while (mlx_get_time() - start < 1.0 / SINGLE_THREAD_TARGET_FPS) {
        if (state->rndr.curr_px < state->screen_width * state->screen_height) {
            t_render_task render_task = {.state = state,
                                         .start_pixel = state->rndr.curr_px,
                                         .prng_state = move_tl_prng(state),
                                         .num_pixels = 1,
                                         .thrd_state = &exit_flag};
            render_step(&render_task);
            state->rndr.curr_px++;
        } else {
            state->rndr.total_runs++;
            state->rndr.curr_px = 0;
            if (state->rndr.exit_after_render)
                exit_app(state);
        }
    }
}
void launch_worker_threads(t_state* state) {
    int i = -1;
    int pos;
    while (state->rndr.curr_px < state->screen_width * state->screen_height &&
           (pos = thread_idx(state, THRD_NONE | THRD_FINISHED)) != -1 &&
           ++i < state->rndr.num_threads) {
        if (state->rndr.thrd_states[pos] == THRD_FINISHED)
            pthread_join(state->rndr.threads[pos], NULL);
        state->rndr.thrd_states[pos] = THRD_RUNNING;
        state->rndr.tasks[pos] =
            (t_render_task){.state = state,
                            .start_pixel = state->rndr.curr_px,
                            .num_pixels = state->rndr.chunk_size,
                            .prng_state = move_tl_prng(state),
                            .thrd_state = &state->rndr.thrd_states[pos]};
        state->rndr.curr_px += state->rndr.chunk_size;
        ft_printf("creating thread...\n");
        pthread_create(&state->rndr.threads[pos], NULL, render_step,
                       &state->rndr.tasks[pos]);
    }
    ft_printf("exited looop\n");
}

void render_multithread(t_state* state) {
    if (state->rndr.curr_px == 0 && state->rndr.render_once &&
        state->rndr.total_runs > 0)
        return;
    if (state->rndr.curr_px < state->screen_width * state->screen_height) {
        launch_worker_threads(state);
    } else {
        int curr;
        while ((curr = thread_idx(state, THRD_FINISHED)) != -1) {
            ft_printf("joining thread\n");
            pthread_join(state->rndr.threads[curr], NULL);
            state->rndr.thrd_states[curr] = THRD_NONE;
        }
        if (get_num_threads(state, THRD_NONE) == state->rndr.num_threads) {
            ft_printf("resetting everything\n");
            ft_memset(state->rndr.thrd_states, THRD_NONE,
                      sizeof(state->rndr.thrd_states));
            state->rndr.curr_px = 0;
            state->rndr.total_runs++;
            draw(state);
			if (state->output_path.len)
				write_image_to_ppm(state);
            if (state->rndr.exit_after_render)
                exit_app(state);
        }
    }
}

void emergency_exit(t_state* state) {
    for (int i = 0; i < state->rndr.num_threads; i++) {
        state->rndr.tasks[i].exit_immediatelly = 1;
    }
    int curr;
    while ((curr = thread_idx(state, THRD_RUNNING | THRD_FINISHED)) != -1) {
        pthread_join(state->rndr.threads[curr], NULL);
        state->rndr.thrd_states[curr] = THRD_NONE;
    }
}

void exit_hook(void* state_arg) {
    t_state* state = (t_state*)state_arg;
    emergency_exit(state);
}

void loop_hook(void* state_param) {
    t_state* state = (t_state*)state_param;
    printf("Loop hook: %lf\n", mlx_get_time());

    render_multithread(state);
    // render_single_thread(state);
    draw(state);
}
