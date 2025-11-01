/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consumer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 12:44:16 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 17:17:29 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"
#include "rt_parser.h"
#include "rt_consumer.h"
#include "rt_tokenizer.h"
#include "../libft/libft.h"
#include "rt_types.h"
#include "../cie.h"
#include <limits.h>

// 0 nothing
// 1 ok
// 2 diff type
int	get_tl_typed(t_rt_consumer_tl *tl,
				char *name,
				enum e_RT_NODE_TYPE t,
				t_rt_node *ret)
{
	t_rt_kv	*kv;

	tl->consumer->expected = t;
	tl->consumer->last_node = tl->kv->v;
	tl->consumer->last_key = tl->kv->k;
	tl->consumer->last_ident = name;
	tl->kv->used = true;
	if (tl->kv->v.t == RT_ND_DICT)
	{
		tl->kv->v.used = true;
		kv = vec_rt_kv_get_str(&tl->kv->v.dict,
				tl->consumer->parser.tokenizer.file.contents.buff, name);
		if (!kv)
			return (tl->consumer->err = RT_ERR_NOT_FOUND_DICT, 0);
		kv->used = true;
		kv->v.used = true;
		tl->consumer->last_node = kv->v;
		if (rt_type_cmp(t, kv->v.t))
			return (*ret = kv->v, 1);
		return (tl->consumer->err = RT_ERR_INVALID_TYPE, 2);
	}
	else
	{
		ft_assert(tl->kv->v.t == RT_ND_LIST);
		if (tl->kv->v.list.len <= (size_t)tl->consumer->curr_idx)
			return (tl->consumer->err = RT_ERR_ARR_TOO_SHORT, 0);
		tl->kv->v.list.buff[tl->consumer->curr_idx].used = true;
		tl->consumer->last_node = tl->kv->v.list.buff[tl->consumer->curr_idx];
		if (!rt_type_cmp(t, tl->consumer->last_node.t))
			return (tl->consumer->err = RT_ERR_INVALID_TYPE, 2);
		tl->consumer->curr_idx++;
		return (*ret = tl->consumer->last_node, 1);
	}
}

bool	check_range(t_rt_consumer *consumer, t_rt_node nd, float min, float max)
{
	int	i;

	ft_assert(nd.token.t == RT_TUPLE);
	consumer->last_node = nd;
	consumer->range_start = min;
	consumer->range_end = max;
	i = 0;
	while (i < nd.token.tuple_len)
	{
		if (nd.token.vals_f[i] < min || nd.token.vals_f[i] > max)
		{
			consumer->err = RT_ERR_OUT_OF_RANGE;
			return (false);
		}
		i++;
	}
	return (true);
}

t_fvec3	get_fvec3(t_rt_token tk)
{
	t_fvec3	ret;

	ft_assert(tk.tuple_len == 3);
	ret.x = tk.vals_f[0];
	ret.y = tk.vals_f[1];
	ret.z = tk.vals_f[2];
	return (ret);
}

float	get_float(t_rt_token tk)
{
	ft_assert(tk.tuple_len == 1);
	return (tk.vals_f[0]);
}

t_dyn_str	get_string(t_rt_consumer *consumer, t_rt_token tk)
{
	t_dyn_str	ret;

	ret = (t_dyn_str){0};
	ft_assert(tk.t == RT_STRING);
	dyn_str_pushnstr(&ret, tk.start_idx
		+ consumer->parser.tokenizer.file.contents.buff + 1, tk.len - 2);
	ft_printf("buf: %s\n", ret.buff);
	return (ret);
}

bool	get_bool(t_rt_token tk)
{
	ft_assert(tk.t == RT_BOOL);
	return (tk.vals_f[0] != 0);
}

t_fvec2	get_fvec2(t_rt_token tk)
{
	ft_assert(tk.tuple_len == 2);
	return ((t_fvec2){.x = tk.vals_f[0], .y = tk.vals_f[1]});
}

int push_color(t_rt_token tk, t_state *state, bool clamp)
{
	t_fvec3	color;
	t_fvec3	xyz;

	color = get_fvec3(tk);
	xyz = rgb_to_xyz((t_8bcolor){color.x, color.y, color.z});
	vec_densely_sampled_spectrum_push(&state->spectrums,
		xyz_to_spectrum(xyz, clamp, 0));
	return (state->spectrums.len - 1);
}

int	push_blackbody(t_rt_token tk, t_state *state)
{
	float	temperature;

	temperature = get_float(tk);
	vec_densely_sampled_spectrum_push(&state->spectrums,
		calculateDenselySampledSpectrum(temperature));
	return (state->spectrums.len - 1);
}

bool	process_ambiant(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;
	float		scale;
	t_fvec3		color;
	t_fvec3		xyz;
	int			i;

	if (get_tl_typed(tl, "lighting_ratio", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, INFINITY))
		return (false);
	scale = get_float(nd.token);
	if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
	color = get_fvec3(nd.token);
	xyz = rgb_to_xyz((t_8bcolor){color.x, color.y, color.z});
	tl->state->ambiant_light_spec = xyz_to_spectrum(xyz, false, 0);
	i = 0;
	while (i < CIE_SAMPLES)
	{
		tl->state->ambiant_light_spec.samples[i] *= scale;
		i++;
	}
	return (true);
}

bool	process_sky(t_rt_consumer_tl *tl)
{
	t_rt_node	nd;
	t_light		l;

	if (get_tl_typed(tl, "lighting_ratio", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, INFINITY))
		return (false);
	l.intensity = get_float(nd.token);
	if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
	l.t = AMBIANT_LIGHT;
	l.spec_idx = push_color(nd.token, tl->state, false);
	tl->state->sky_light_idx = tl->state->lights.lights.len;
	add_light(&tl->state->lights, l);
	return (true);
}

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
	tl->state->cam.pos = get_fvec3(nd.token);
	if (get_tl_typed(tl, "direction", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	tl->state->cam.dir = fvec3_norm(get_fvec3(nd.token));
	tl->state->cam_yaw = atan2f(tl->state->cam.dir.x, tl->state->cam.dir.y);
	tl->state->cam_pitch = asinf(-tl->state->cam.dir.z);
	if (get_tl_typed(tl, "fov", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 180))
		return (false);
	tl->state->fov = get_float(nd.token);
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

bool	process_light_color(t_rt_consumer_tl *tl, bool is_blackbody, t_light *l)
{
	t_rt_node	nd;

	if (is_blackbody)
	{
		if (get_tl_typed(tl, "temperature", RT_ND_TUPLE_F1, &nd) != 1
			|| !check_range(tl->consumer, nd, 0, 100000))
			return (false);
		l->spec_idx = push_blackbody(nd.token, tl->state);
	}
	else
	{
		if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
			|| !check_range(tl->consumer, nd, 0, 255))
			return (false);
		l->spec_idx = push_color(nd.token, tl->state, false);
	}
	return (true);
}

bool	process_light(t_rt_consumer_tl *tl, bool is_blackbody)
{
	t_rt_node	nd;
	t_light		l;

	l = (t_light){.t = POINT_LIGHT};
	if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	l.position = get_fvec3(nd.token);
	if (get_tl_typed(tl, "brightness", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, INFINITY))
		return (false);
	l.intensity = get_float(nd.token);
	if (!process_light_color(tl, is_blackbody, &l))
		return (false);
	add_light(&tl->state->lights, l);
	return (true);
}

bool	process_inf_light(t_rt_consumer_tl* tl, bool is_blackbody)
{
	t_rt_node	nd;
	t_light		l;

	l = (t_light){.t = DISTANT_LIGHT};
	if (get_tl_typed(tl, "direction", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	l.position = get_fvec3(nd.token);
	if (get_tl_typed(tl, "brightness", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, INFINITY))
		return (false);
	l.intensity = get_float(nd.token);
	if (!process_light_color(tl, is_blackbody, &l))
		return (false);
	add_light(&tl->state->lights, l);
	return (true);
}

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

bool process_obj_opts(t_rt_consumer_tl *tl, t_obj_spec *s)
{
	t_rt_node	nd;
	int			ret;

	if (get_tl_typed(tl, "path", RT_ND_STRING, &nd) != 1)
		return (false);
	s->path = get_string(tl->consumer, nd.token).buff;
	printf("path: %s\n", s->path);
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

bool	process_kv_items(t_rt_consumer_tl *tl)
{
	char	*buff;

	buff = tl->consumer->parser.tokenizer.file.contents.buff;
	if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "A"))
		return (process_ambiant(tl));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "Sky"))
		return (process_sky(tl));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "C"))
		return (process_camera(tl));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "L"))
		return (process_light(tl, false));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "l"))
		return (process_light(tl, false));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len,
			"blackbody"))
		return (process_light(tl, true));
	else
		return (false);
}

bool	process_kv_items2(t_rt_consumer_tl *tl)
{
	char	*buff;

	buff = tl->consumer->parser.tokenizer.file.contents.buff;
	if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "distant"))
		return (process_inf_light(tl, false));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len,
			"distant_blackbody"))
		return (process_inf_light(tl, true));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "pl"))
		return (process_plane(tl));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "sp"))
		return (process_sphere(tl));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "cy"))
		return (process_cylinder(tl));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "obj"))
		return (process_obj(tl));
	return (true);
}

bool	process_kv(t_rt_consumer* consumer, t_state* state, t_rt_kv *kv)
{
	t_rt_consumer_tl	tl;

	tl = (t_rt_consumer_tl){.consumer = consumer, .kv = kv, .state = state};
	tl.consumer->curr_idx = 0;
	kv->v.used = true;
	kv->used = true;
	if (!process_kv_items(&tl) && !process_kv_items2(&tl))
	{
		kv->v.used = false;
		kv->used = false;
		consumer->last_key = kv->k;
		consumer->err = RT_ERR_KEY_NOT_USED;
		return (false);
	}
	return (true);
}

bool	check_unused(t_rt_consumer *consumer, t_rt_node nd);

bool	check_unused_dict(t_rt_consumer *consumer, t_vec_rt_kv dict)
{
	size_t	i;

	i = 0;
	while (i < dict.len)
	{
		if (!dict.buff[i].used)
		{
			consumer->err = RT_ERR_KEY_NOT_USED;
			consumer->last_key = dict.buff[i].k;
			return (false);
		}
		if (!check_unused(consumer, dict.buff[i].v))
			return (false);
		i++;
	}
	return (true);
}

bool	check_unused(t_rt_consumer *consumer, t_rt_node nd)
{
	size_t	i;

	if (!nd.used)
	{
		consumer->err = RT_ERR_NODE_NOT_USED;
		consumer->last_node = nd;
		return (false);
	}
	i = 0;
	while (i < nd.list.len)
	{
		if (!check_unused(consumer, nd.list.buff[i]))
			return (false);
		i++;
	}
	if (!check_unused_dict(consumer, nd.dict))
		return (false);
	return (true);
}

bool	consume_parsed_nodes(t_rt_parser parser, t_state *state)
{
	t_rt_consumer	consumer;
	size_t			i;

	consumer = (t_rt_consumer){.parser = parser};
	i = 0;
	while (i < parser.statements.len)
	{
		if (!process_kv(&consumer, state, &parser.statements.buff[i]))
		{
			print_consumer_err(&consumer);
			return (false);
		}
		if (!check_unused(&consumer, parser.statements.buff[i].v))
		{
			print_consumer_err(&consumer);
			return (false);
		}
		i++;
	}
	return (true);
}
