/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consumer_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 08:38:53 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 08:38:53 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_consumer.h"
#include "rt_parser.h"
#include <stdbool.h>

bool	process_kv_items(t_rt_consumer_tl *tl, bool *succ)
{
	char	*buff;

	buff = tl->consumer->parser.tokenizer.file.contents.buff;
	if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "A"))
		*succ = process_ambiant(tl);
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "Sky"))
		*succ = (process_sky(tl));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "C"))
		*succ = (process_camera(tl));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "L"))
		*succ = (process_light(tl, false));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "l"))
		*succ = (process_light(tl, false));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len,
			"blackbody"))
		*succ = (process_light(tl, true));
	else
		return (false);
	return (true);
}

bool	process_kv_items2(t_rt_consumer_tl *tl, bool *succ)
{
	char	*buff;

	buff = tl->consumer->parser.tokenizer.file.contents.buff;
	if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "distant"))
		*succ = (process_inf_light(tl, false));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len,
			"distant_blackbody"))
		*succ = (process_inf_light(tl, true));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "pl"))
		*succ = (process_plane(tl));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "sp"))
		*succ = (process_sphere(tl));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "cy"))
		*succ = (process_cylinder(tl));
	else if (str_slice_eq_str(buff + tl->kv->k.start_idx, tl->kv->k.len, "obj"))
		*succ = (process_obj(tl));
	else
		return (false);
	return (true);
}

bool	process_kv(t_rt_consumer *consumer, t_state *state, t_rt_kv *kv)
{
	t_rt_consumer_tl	tl;
	bool				succ;

	tl = (t_rt_consumer_tl){.consumer = consumer, .kv = kv, .state = state};
	tl.consumer->curr_idx = 0;
	kv->v.used = true;
	kv->used = true;
	succ = true;
	if (process_kv_items(&tl, &succ))
		return (succ);
	if (process_kv_items2(&tl, &succ))
		return (succ);
	kv->v.used = false;
	kv->used = false;
	consumer->last_key = kv->k;
	consumer->err = RT_ERR_KEY_NOT_USED;
	return (false);
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
