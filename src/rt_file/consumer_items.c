/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consumer_items.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 08:39:30 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 08:39:30 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_consumer.h"

bool	process_sphere(t_rt_consumer_tl *tl)
{
	t_sphere	sp;
	t_rt_node	nd;

	if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	sp.p = get_fvec3(nd.token);
	if (get_tl_typed(tl, "diameter", RT_ND_TUPLE_F1, &nd) != 1)
		return (false);
	sp.r = get_float(nd.token);
	if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
	sp.spectrum_idx = push_color(nd.token, tl->state, true);
	vec_sphere_push(&tl->state->spheres, sp);
	return (true);
}

bool	process_plane(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;
	t_plane		pl;

	pl = (t_plane){0};
	if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	pl.pos = get_fvec3(nd.token);
	if (get_tl_typed(tl, "direction", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	pl.dir = fvec3_norm(get_fvec3(nd.token));
	if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
	pl.spectrum_idx = push_color(nd.token, tl->state, true);
	vec_plane_push(&tl->state->planes, pl);
	return (true);
}

static bool	process_obj_opts(t_rt_consumer_tl *tl, t_obj_spec *s)
{
	t_rt_node	nd;
	int			ret;

	if (get_tl_typed(tl, "path", RT_ND_STRING, &nd) != 1)
		return (false);
	s->path = get_string(tl->consumer, nd.token).buff;
	if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	s->pos = get_fvec3(nd.token);
	if (get_tl_typed(tl, "scale", RT_ND_TUPLE_F1, &nd) != 1)
		return (false);
	s->scale = get_float(nd.token);
	ret = get_tl_typed(tl, "rotation_yaw_pitch", RT_ND_TUPLE_F2, &nd);
	if (ret == 2)
		return (false);
	if (ret == 1)
		s->rotation = get_fvec2(nd.token);
	ret = get_tl_typed(tl, "forward_z", RT_ND_BOOL, &nd);
	if (ret == 2)
		return (false);
	if (ret == 1)
		s->forward_z = get_bool(nd.token);
	return (true);
}

bool	process_obj(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;
	t_obj_spec	s;

	s = (t_obj_spec){.forward_z = true};
	if (!process_obj_opts(tl, &s))
		return (false);
	if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
	s.spectrum_idx = push_color(nd.token, tl->state, true);
	if (!load_triangles(tl->state, s))
	{
		free(s.path);
		tl->consumer->last_key = tl->kv->k;
		tl->consumer->err = RT_ERR_FAILED_PROCESSING_KEY;
		return (false);
	}
	free(s.path);
	return (true);
}

bool	process_cylinder(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;
	t_cylinder	cylinder;
	t_fvec3		dir;
	float		height;

	cylinder = (t_cylinder){0};
	if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	cylinder.a = get_fvec3(nd.token);
	if (get_tl_typed(tl, "direction", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	dir = fvec3_norm(get_fvec3(nd.token));
	if (get_tl_typed(tl, "diameter", RT_ND_TUPLE_F1, &nd) != 1)
		return (false);
	cylinder.radius = get_float(nd.token) / 2.0;
	if (get_tl_typed(tl, "height", RT_ND_TUPLE_F1, &nd) != 1)
		return (false);
	height = get_float(nd.token);
	if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
	cylinder.spectrum_idx = push_color(nd.token, tl->state, true);
	cylinder.b = fvec3_add(fvec3_scale(dir, height), cylinder.a);
	vec_cylinder_push(&tl->state->cylinders, cylinder);
	return (true);
}
