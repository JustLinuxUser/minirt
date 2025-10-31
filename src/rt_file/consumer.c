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
int get_tl_typed(t_rt_consumer_tl* tl,
                 char* name,
                 enum RT_NODE_TYPE t,
                 t_rt_node* ret) {
    t_rt_kv* kv;

    tl->consumer->expected = t;
	tl->consumer->last_node = tl->kv->v;
	tl->consumer->last_key = tl->kv->k;
	tl->consumer->last_ident = name;
	tl->consumer->last_idx = tl->i;
	tl->kv->used = true;
    if (tl->kv->v.t == RT_ND_DICT) {
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
    } else {
        ft_assert(tl->kv->v.t == RT_ND_LIST);
        if (tl->kv->v.list.len <= (size_t)tl->i)
            return (tl->consumer->err = RT_ERR_ARR_TOO_SHORT, 0);
		tl->kv->v.list.buff[tl->i].used = true;
        tl->consumer->last_node = tl->kv->v.list.buff[tl->i];
        if (!rt_type_cmp(t, tl->consumer->last_node.t))
            return (tl->consumer->err = RT_ERR_INVALID_TYPE, 2);
        tl->i++;
        return (*ret = tl->consumer->last_node, 1);
    }
}

bool check_range(t_rt_consumer *consumer, t_rt_node nd, float min, float max) {
	ft_assert(nd.token.t == RT_TUPLE);

	consumer->last_node = nd;
	consumer->range_start = min;
	consumer->range_end = max;
	for (int i = 0; i < nd.token.tuple_len; i++) {
		if (nd.token.vals_f[i] < min || nd.token.vals_f[i] > max) {
			consumer->err = RT_ERR_OUT_OF_RANGE;
			return (false);
		}
	}
	return (true);
}

t_fvec3 get_fvec3(t_rt_token tk) {
	t_fvec3 ret;

	ft_assert(tk.tuple_len == 3);
	ret.x = tk.vals_f[0];
	ret.y = tk.vals_f[1];
	ret.z = tk.vals_f[2];
	return (ret);
}

float get_float(t_rt_token tk) {
	ft_assert(tk.tuple_len == 1);
	return (tk.vals_f[0]);
}

bool get_bool(t_rt_token tk) {
	ft_assert(tk.t == RT_BOOL);
	return (tk.vals_f[0] != 0);
}

t_fvec2 get_fvec2(t_rt_token tk) {
	ft_assert(tk.tuple_len == 2);
	return ((t_fvec2){.x = tk.vals_f[0], .y = tk.vals_f[1]});
}

int push_color(t_rt_token tk, t_state *state, bool clamp) {
	t_fvec3 color = get_fvec3(tk);
	t_fvec3 xyz = rgb_to_xyz((t_8bcolor){color.x, color.y, color.z});

	vec_densely_sampled_spectrum_push(&state->spectrums, xyz_to_spectrum(xyz, clamp, 0));
	return (state->spectrums.len - 1);
}

int push_blackbody(t_rt_token tk, t_state *state) {
	float temperature = get_float(tk);

	vec_densely_sampled_spectrum_push(&state->spectrums, calculateDenselySampledSpectrum(temperature));
	return (state->spectrums.len - 1);
}


bool process_ambiant(t_rt_consumer_tl* tl) {
    t_rt_node nd;

    if (get_tl_typed(tl, "lighting_ratio", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, INFINITY))
		return (false);
	float scale = get_float(nd.token);

    if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);

	t_fvec3 color = get_fvec3(nd.token);
	t_fvec3 xyz = rgb_to_xyz((t_8bcolor){color.x, color.y, color.z});

	tl->state->ambiant_light_spec = xyz_to_spectrum(xyz, false, 0);
	for (int i = 0; i < CIE_SAMPLES; i++) {
		tl->state->ambiant_light_spec.samples[i] *= scale;
	}
    return (true);
}

bool process_sky(t_rt_consumer_tl* tl) {
    t_rt_node nd;

    if (get_tl_typed(tl, "lighting_ratio", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, INFINITY))
		return (false);
	float scale = get_float(nd.token);

    if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);

	t_fvec3 color = get_fvec3(nd.token);
	t_fvec3 xyz = rgb_to_xyz((t_8bcolor){color.x, color.y, color.z});

	tl->state->sky_spec = xyz_to_spectrum(xyz, false, 0);
	for (int i = 0; i < CIE_SAMPLES; i++) {
		tl->state->sky_spec.samples[i] *= scale;
	}
    return (true);
}

bool process_camera(t_rt_consumer_tl* tl) {
    t_rt_node nd;
	int		ret;
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

	ret = get_tl_typed(tl, "window_size", RT_ND_TUPLE_I2, &nd);
	if (ret != 0) {
		if (ret != 1 || !check_range(tl->consumer, nd, 50, 5000))
			return (false);
		t_fvec2 size = get_fvec2(nd.token);
		tl->state->screen_width = size.x;
		tl->state->screen_height = size.y;
	}
	ret = get_tl_typed(tl, "samples", RT_ND_TUPLE_I2, &nd);
	if (ret != 0) {
		if (ret != 1 || !check_range(tl->consumer, nd, 1, 1000))
			return (false);
		t_fvec2 size = get_fvec2(nd.token);
		tl->state->samples_x = size.x;
		tl->state->samples_y = size.y;
	}
	ret = get_tl_typed(tl, "num_threads", RT_ND_TUPLE_I1, &nd);
	if (ret != 0) {
		if (ret != 1 || !check_range(tl->consumer, nd, 1, MAX_NUM_THREADS))
			return (false);
		tl->state->rndr.num_threads = get_float(nd.token);
	}
	ret = get_tl_typed(tl, "rndr_chunk_size", RT_ND_TUPLE_I1, &nd);
	if (ret != 0) {
		if (ret != 1 || !check_range(tl->consumer, nd, 1, 16777216))
			return (false);
		tl->state->rndr.chunk_size = get_float(nd.token);
	}
	ret = get_tl_typed(tl, "max_reflections", RT_ND_TUPLE_I1, &nd);
	if (ret != 0) {
		if (ret != 1 || !check_range(tl->consumer, nd, 1, 200))
			return (false);
		tl->state->rndr.max_reflections = get_float(nd.token);
	}

	ret = get_tl_typed(tl, "render_once", RT_ND_BOOL, &nd);
	if (ret != 0) {
		if (ret != 1)
			return (false);
		tl->state->rndr.render_once = get_bool(nd.token);
	}
	ret = get_tl_typed(tl, "exit_after_render", RT_ND_BOOL, &nd);
	if (ret != 0) {
		if (ret != 1)
			return (false);
		tl->state->rndr.exit_after_render = get_bool(nd.token);
	}
    return (true);
}

bool process_light(t_rt_consumer_tl* tl, bool is_blackbody) {
    t_rt_node nd;
	t_light l = {.t = POINT_LIGHT };
    if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	l.position = get_fvec3(nd.token);

    if (get_tl_typed(tl, "brightness", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, INFINITY))
		return (false);
	l.intensity = get_float(nd.token);

	if (is_blackbody) {
		if (get_tl_typed(tl, "temperature", RT_ND_TUPLE_F1, &nd) != 1
			|| !check_range(tl->consumer, nd, 0, 100000))
			return (false);
		l.spec_idx = push_blackbody(nd.token, tl->state);
	}else {
		if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
			|| !check_range(tl->consumer, nd, 0, 255))
			return (false);
		l.spec_idx = push_color(nd.token, tl->state, false);
	}
	add_light(&tl->state->lights, l);
    return (true);
}

bool process_inf_light(t_rt_consumer_tl* tl, bool is_blackbody) {
    t_rt_node nd;
	t_light l = {.t = DISTANT_LIGHT };
    if (get_tl_typed(tl, "direction", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	l.position = get_fvec3(nd.token);

    if (get_tl_typed(tl, "brightness", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, INFINITY))
		return (false);
	l.intensity = get_float(nd.token);

	if (is_blackbody) {
		if (get_tl_typed(tl, "temperature", RT_ND_TUPLE_F1, &nd) != 1
			|| !check_range(tl->consumer, nd, 0, 100000))
			return (false);
		l.spec_idx = push_blackbody(nd.token, tl->state);
	}else {
		if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
			|| !check_range(tl->consumer, nd, 0, 255))
			return (false);
		l.spec_idx = push_color(nd.token, tl->state, false);
	}
	add_light(&tl->state->lights, l);
    return (true);
}

bool process_sphere(t_rt_consumer_tl* tl) {
	t_sphere sp;

    t_rt_node nd;
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

bool process_plane(t_rt_consumer_tl* tl) {
    t_rt_node nd;
	t_plane pl = {0};

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

bool process_obj(t_rt_consumer_tl* tl) {
    t_rt_node nd;

    if (get_tl_typed(tl, "path", RT_ND_STRING, &nd) != 1)
		return (false);

	char *path = ft_strndup(tl->consumer->parser.tokenizer.file.contents.buff + nd.token.start_idx + 1, nd.token.len - 2);

    if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);

	t_fvec3 pos = get_fvec3(nd.token);

    if (get_tl_typed(tl, "scale", RT_ND_TUPLE_F1, &nd) != 1)
		return (false);
	float scale = get_float(nd.token);

	t_fvec2 rotation = {0};
	int ret = get_tl_typed(tl, "rotation_yaw_pitch", RT_ND_TUPLE_F2, &nd);
    if (ret == 2)
		return (false);
	if (ret == 1)
		rotation = get_fvec2(nd.token);

    if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);

	bool forward_z = true;
	ret = get_tl_typed(tl, "forward_z", RT_ND_BOOL, &nd);
	if (ret != 0)
	{
		if (ret != 1)
			return (false);
		forward_z = get_bool(nd.token);
	}

	int color_idx = push_color(nd.token, tl->state, true);
	if (!load_triangles(tl->state, path, pos, scale, rotation, color_idx, forward_z)) {
		free(path);
		tl->consumer->last_key = tl->kv->k;
		tl->consumer->err = RT_ERR_FAILED_PROCESSING_KEY;
		return (false);
	}
	free(path);

    return (true);
}

bool process_cylinder(t_rt_consumer_tl* tl) {
    t_rt_node nd;
	t_cylinder cylinder = {0};

    if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	cylinder.a = get_fvec3(nd.token);

    if (get_tl_typed(tl, "direction", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);
	t_fvec3 dir = fvec3_norm(get_fvec3(nd.token));

    if (get_tl_typed(tl, "diameter", RT_ND_TUPLE_F1, &nd) != 1)
		return (false);
	cylinder.radius = get_float(nd.token) / 2.0;

    if (get_tl_typed(tl, "height", RT_ND_TUPLE_F1, &nd) != 1)
		return (false);
	float height = get_float(nd.token);

    if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
	cylinder.spectrum_idx = push_color(nd.token, tl->state, true);

	cylinder.b = fvec3_add(fvec3_scale(dir, height), cylinder.a);
	vec_cylinder_push(&tl->state->cylinders, cylinder);
    return (true);
}

bool process_kv(t_rt_consumer* consumer, t_state* state, t_rt_kv *kv) {
    char* buff;
    t_rt_consumer_tl tl = {.consumer = consumer, .kv = kv, .state = state};

    buff = consumer->parser.tokenizer.file.contents.buff;
	kv->v.used = true;
	kv->used = true;
    // A
    // C
    // L
    // sp
    // pl
    // cy
    if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "A")) {
        return process_ambiant(&tl);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "Sky")) {
        return process_sky(&tl);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "C")) {
        return process_camera(&tl);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "L")) {
        return process_light(&tl, false);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "l")) {
        return process_light(&tl, false);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "blackbody")) {
        return process_light(&tl, true);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "distant")) {
        return process_inf_light(&tl, false);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "distant_blackbody")) {
        return process_inf_light(&tl, true);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "pl")) {
        return process_plane(&tl);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "sp")) {
        return process_sphere(&tl);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "cy")) {
        return process_cylinder(&tl);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "obj")) {
        return process_obj(&tl);
    } else {
		kv->v.used = false;
		kv->used = false;
		consumer->last_key = kv->k;
		consumer->err = RT_ERR_KEY_NOT_USED;
		return (false);
    }
    return (true);
}

bool check_unused(t_rt_consumer *consumer, t_rt_node nd) {
	if (!nd.used)
	{
		consumer->err = RT_ERR_NODE_NOT_USED;
		consumer->last_node = nd;
		return (false);
	}
	for (size_t i = 0; i < nd.dict.len; i++) {
		if (!nd.dict.buff[i].used)
		{
			consumer->err = RT_ERR_KEY_NOT_USED;
			consumer->last_key = nd.dict.buff[i].k;
			return (false);
		}
		if (!check_unused(consumer, nd.dict.buff[i].v))
			return (false);
	}
	for (size_t i = 0; i < nd.list.len; i++) {
		if (!check_unused(consumer, nd.list.buff[i]))
			return (false);
	}
	return (true);
}

bool consume_parsed_nodes(t_rt_parser parser, t_state* state) {
    t_rt_consumer consumer;

    consumer = (t_rt_consumer){.parser = parser};
    for (size_t i = 0; i < parser.statements.len; i++) {
        if (!process_kv(&consumer, state, &parser.statements.buff[i]))
		{
			print_consumer_err(&consumer);
			return (false);
		}
		if (!check_unused(&consumer, parser.statements.buff[i].v)) {
			print_consumer_err(&consumer);
			return false;
		}
    }
    return (true);
}
