#include "../minirt.h"
#include "rt_parser.h"
#include "rt_consumer.h"
#include "rt_tokenizer.h"
#include "../libft/libft.h"
#include "rt_types.h"

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
                               tl->consumer->parser.tokenizer.file.buff, name);
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
            return (tl->consumer->err = RT_ERR_NOT_FOUND_ARR, 0);
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

bool process_ambiant(t_rt_consumer_tl* tl) {
    t_rt_node nd;
    if (get_tl_typed(tl, "lighting_ratio", RT_ND_TUPLE_F1, &nd) != 1)
		return (false);

    if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
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

bool process_camera(t_rt_consumer_tl* tl) {
    t_rt_node nd;
    if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);

	tl->state->cam.pos = get_fvec3(nd.token);

	// TODO: check if normalizable, or gracefully fallback
    if (get_tl_typed(tl, "direction", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);

	tl->state->cam.dir = get_fvec3(nd.token);

    if (get_tl_typed(tl, "fov", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 180))
		return (false);
    return (true);
}

bool process_light(t_rt_consumer_tl* tl) {
    t_rt_node nd;
    if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);

    if (get_tl_typed(tl, "brightness", RT_ND_TUPLE_F1, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 1))
		return (false);

    if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
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

	vec_sphere_push(&tl->state->spheres, sp);

    if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
    return (true);
}

bool process_plane(t_rt_consumer_tl* tl) {
    t_rt_node nd;
	t_plane pl = {0};

    if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);

	pl.pos = get_fvec3(nd.token);

	// TODO: check if normalizable, or gracefully fallback
    if (get_tl_typed(tl, "direction", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);

	pl.dir = fvec3_norm(get_fvec3(nd.token));

    if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);

	vec_plane_push(&tl->state->planes, pl);
    return (true);
}

bool process_obj(t_rt_consumer_tl* tl) {
    t_rt_node nd;

    if (get_tl_typed(tl, "path", RT_ND_STRING, &nd) != 1)
		return (false);

	char *path = ft_strndup(tl->consumer->parser.tokenizer.file.buff + nd.token.start_idx + 1, nd.token.len - 2);

	// TODO: check if normalizable, or gracefully fallback
    if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);

	t_fvec3 pos = get_fvec3(nd.token);

    if (get_tl_typed(tl, "scale", RT_ND_TUPLE_F1, &nd) != 1)
		return (false);
	float scale = get_float(nd.token);

	load_triangles(tl->state, path, pos, scale);
	free(path);
    return (true);
}

bool process_cylinder(t_rt_consumer_tl* tl) {
    t_rt_node nd;

    if (get_tl_typed(tl, "position", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);

	// TODO: check if normalizable, or gracefully fallback
    if (get_tl_typed(tl, "direction", RT_ND_TUPLE_F3, &nd) != 1)
		return (false);

    if (get_tl_typed(tl, "dieameter", RT_ND_TUPLE_F1, &nd) != 1)
		return (false);

    if (get_tl_typed(tl, "height", RT_ND_TUPLE_F1, &nd) != 1)
		return (false);

    if (get_tl_typed(tl, "color", RT_ND_TUPLE_I3, &nd) != 1
		|| !check_range(tl->consumer, nd, 0, 255))
		return (false);
    return (true);
}

// what do we need in a tl get_type?
// - ptr to idx, starts at 0
// - kv / node
// - target type
// - target name (string)
// return
//
// TODO: Make sure no duplicates in the dict
//

bool process_kv(t_rt_consumer* consumer, t_state* state, t_rt_kv *kv) {
    char* buff;
    t_rt_consumer_tl tl = {.consumer = consumer, .kv = kv, .state = state};

    buff = consumer->parser.tokenizer.file.buff;
    // A
    // C
    // L
    // sp
    // pl
    // cy
	kv->v.used = true;
	kv->used = true;
    if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "A")) {
        return process_ambiant(&tl);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "C")) {
        return process_camera(&tl);
    } else if (str_slice_eq_str(buff + kv->k.start_idx, kv->k.len, "L")) {
        return process_light(&tl);
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
		consumer->last_key = kv->k;
		consumer->err = RT_ERR_KEY_NOT_USED;
        printf("Unknown value\n");
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
			// ft_printf("Hello\n\n\n");
			// exit (1);
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
			break;
		}
		if (!check_unused(&consumer, parser.statements.buff[i].v)) {
			print_consumer_err(&consumer);
			return false;
		}
    }
    return (true);
}
