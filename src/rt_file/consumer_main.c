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

bool	process_kv(t_rt_consumer *consumer, t_state *state, t_rt_kv *kv)
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
