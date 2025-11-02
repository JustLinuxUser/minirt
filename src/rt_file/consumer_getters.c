/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consumer_getters.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 08:39:41 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 08:39:41 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_consumer.h"
#include "rt_types.h"

static int	get_tl_arr(t_rt_consumer_tl *tl, enum e_RT_NODE_TYPE t,
				t_rt_node *ret)
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
		return (get_tl_arr(tl, t, ret));
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
	return (ret);
}
