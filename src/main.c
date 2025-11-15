/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 15:15:39 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/07 15:15:39 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bvh.h"
#include "libft/alloc/mmalloc.h"
#include "minirt.h"
#include "mymath.h"
#include "rt_file/rt_consumer.h"
#include "error.h"
#include "libft/libft.h"
#include "MLX42/MLX42.h"
#include <pthread.h>

void	init_camera(t_state *state)
{
	float	fov_x;
	float	proj_plane_width;

	fov_x = state->cam.fov / 180 * PI;
	proj_plane_width = 2 * tan(fov_x / 2) * state->cam.screen_dist;
	state->cam.proj_coef = proj_plane_width / state->screen_width;
	state->cam.x0y0 = vec3_rotate_yaw_pitch(
			perspective_cam_ray(state, (t_fvec2){0}, (t_fvec2){0}),
			state->cam.cam_yaw, state->cam.cam_pitch);
	state->cam.x1y0 = vec3_rotate_yaw_pitch(
			perspective_cam_ray(state, (t_fvec2){.x = state->screen_width},
				(t_fvec2){0}), state->cam.cam_yaw, state->cam.cam_pitch);
	state->cam.x0y1 = vec3_rotate_yaw_pitch(
			perspective_cam_ray(state, (t_fvec2){.y = state->screen_height},
				(t_fvec2){0}), state->cam.cam_yaw, state->cam.cam_pitch);
	state->cam.x1y1 = vec3_rotate_yaw_pitch(
			perspective_cam_ray(state, (t_fvec2){.x = state->screen_width,
				.y = state->screen_height}, (t_fvec2){0}),
			state->cam.cam_yaw, state->cam.cam_pitch);
}

void	init(t_state *state)
{
	ft_memset(state->rndr.thrd_states,
		THRD_NONE, sizeof(state->rndr.thrd_states));
	state->mlx
		= mlx_init(state->screen_width, state->screen_height, "MiniRT", false);
	if (!state->mlx)
		critical_error("Failed to initialize mlx");
	state->mlx_image
		= mlx_new_image(state->mlx, state->screen_width, state->screen_height);
	if (!state->mlx_image)
		critical_error("Failed to initialize mlx image buffer");
	if (-1 == mlx_image_to_window(state->mlx, state->mlx_image, 0, 0))
		critical_error("Failed to put image to the window");
	state->s_colors
		= mmalloc(state->screen_width * state->screen_height * sizeof(t_fvec3),
			"allocating the xyz colors buffer");
	init_camera(state);
}

void	load_shapes(t_state *state)
{
	size_t	i;

	i = 0;
	while (i < state->triangles.len)
		vec_shape_push(
			&state->shapes,
			(t_shape){.type = OBJ_TRIANGLE,
			.ptr = state->triangles.buff + i++});
	i = 0;
	while (i < state->spheres.len)
		vec_shape_push(
			&state->shapes,
			(t_shape){.type = OBJ_SPHERE,
			.ptr = state->spheres.buff + i++});
	i = 0;
	while (i < state->cylinders.len)
		vec_shape_push(
			&state->shapes,
			(t_shape){.type = OBJ_CYLINDER,
			.ptr = state->cylinders.buff + i++});
	i = 0;
	while (i < state->planes.len)
		vec_shape_push(
			&state->unbounded_shapes,
			(t_shape){.type = OBJ_PLANE, .ptr = state->planes.buff + i++});
}

t_state	state_default(void)
{
	t_state	state;

	state = (t_state){
		.screen_width = 800,
		.screen_height = 600,
		.samples_x = 5,
		.samples_y = 5,
		.cam = {
		.fov = 90,
		.screen_dist = 1,
	},
		.sky_light_idx = -1,
		.rndr = {
		.num_threads = 8,
		.chunk_size = 1000,
		.max_reflections = 4,
		.render_once = false,
		.exit_after_render = false,
		.rand_state.s = {
		1, 1
	},
	}
	};
	return (state);
}

int	main(int argc, char **argv)
{
	t_state	state;

	if (argc != 2)
		return (1);
	state = state_default();
	if (!process_file(argv[1], &state))
		return (free_state(&state), 1);
	init(&state);
	load_shapes(&state);
	build_bvh(&state);
	ft_printf("triangles: %i\n", (int)state.triangles.len);
	calculate_pdfs(&state.lights);
	create_alias_table(&state.lights);
	mlx_close_hook(state.mlx, exit_hook, &state);
	mlx_loop_hook(state.mlx, loop_hook, &state);
	mlx_loop(state.mlx);
	free_state(&state);
}
