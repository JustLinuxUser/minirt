/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consumer_camera.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 08:40:27 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 08:40:27 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_consumer.h"

bool	process_camera_renderer_settings(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;
	int			ret;
	t_fvec2		smaples;

	ret = get_tl_typed(tl, "samples", RT_ND_TUPLE_I2, &nd);
	if (ret != 0)
	{
		if (ret != 1 || !check_range(tl->consumer, nd, 1, 1000))
			return (false);
		smaples = get_fvec2(nd.token);
		tl->state->samples_x = smaples.x;
		tl->state->samples_y = smaples.y;
	}
	ret = get_tl_typed(tl, "num_threads", RT_ND_TUPLE_I1, &nd);
	if (ret != 0)
	{
		if (ret != 1 || !check_range(tl->consumer, nd, 1, MAX_NUM_THREADS))
			return (false);
		tl->state->rndr.num_threads = get_float(nd.token);
	}
	return (true);
}

bool	process_camera_renderer_settings2(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;
	int			ret;

	ret = get_tl_typed(tl, "rndr_chunk_size", RT_ND_TUPLE_I1, &nd);
	if (ret != 0)
	{
		if (ret != 1 || !check_range(tl->consumer, nd, 1, 16777216))
			return (false);
		tl->state->rndr.chunk_size = get_float(nd.token);
	}
	ret = get_tl_typed(tl, "max_reflections", RT_ND_TUPLE_I1, &nd);
	if (ret != 0)
	{
		if (ret != 1 || !check_range(tl->consumer, nd, 1, 200))
			return (false);
		tl->state->rndr.max_reflections = get_float(nd.token);
	}
	ret = get_tl_typed(tl, "render_once", RT_ND_BOOL, &nd);
	if (ret != 0)
	{
		if (ret != 1)
			return (false);
		tl->state->rndr.render_once = get_bool(nd.token);
	}
	return (true);
}

bool	process_camera_renderer_settings3(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;
	int			ret;

	ret = get_tl_typed(tl, "exit_after_render", RT_ND_BOOL, &nd);
	if (ret != 0)
	{
		if (ret != 1)
			return (false);
		tl->state->rndr.exit_after_render = get_bool(nd.token);
	}
	ret = get_tl_typed(tl, "headless", RT_ND_BOOL, &nd);
	if (ret != 0)
	{
		if (ret != 1)
			return (false);
		tl->state->rndr.headless = get_bool(nd.token);
	}
	return (true);
}

bool	process_camera_general_settings(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;
	int			ret;
	t_fvec2		size;

	ret = get_tl_typed(tl, "window_size", RT_ND_TUPLE_I2, &nd);
	if (ret != 0)
	{
		if (ret != 1 || !check_range(tl->consumer, nd, 50, 5000))
			return (false);
		size = get_fvec2(nd.token);
		tl->state->screen_width = size.x;
		tl->state->screen_height = size.y;
	}
	ret = get_tl_typed(tl, "output_ppm", RT_ND_STRING, &nd);
	if (ret != 0)
	{
		if (ret != 1)
			return (false);
		tl->state->output_path = get_string(tl->consumer, nd.token);
	}
	return (true);
}

bool	process_camera(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;

	if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	tl->state->cam.cam.pos = get_fvec3(nd.token);
	if (get_tl_typed(tl, "direction", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	tl->state->cam.cam.dir = fvec3_norm(get_fvec3(nd.token));
	tl->state->cam.cam_yaw = atan2f(tl->state->cam.cam.dir.x,
			tl->state->cam.cam.dir.y);
	tl->state->cam.cam_pitch = asinf(-tl->state->cam.cam.dir.z);
	if (get_tl_typed(tl, "fov", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 180))
		return (false);
	tl->state->cam.fov = get_float(nd.token);
	if (!process_camera_renderer_settings(tl))
		return (false);
	if (!process_camera_renderer_settings2(tl))
		return (false);
	if (!process_camera_renderer_settings3(tl))
		return (false);
	if (!process_camera_general_settings(tl))
		return (false);
	return (true);
}
