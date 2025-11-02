/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt_consumer.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 12:41:54 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 12:41:54 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_CONSUMER_H
# define RT_CONSUMER_H

# include "rt_parser.h"
# include <stdbool.h>

typedef struct t_rt_consumer
{
	t_rt_parser			parser;

	int					curr_idx;

	// error info
	enum e_RT_ERR_TYPE	err;
	t_rt_node			last_node;
	t_rt_token			last_key;

	enum e_RT_NODE_TYPE	expected;
	float				range_start;
	float				range_end;

	char				*last_ident;
}	t_rt_consumer;

typedef struct t_rt_consumer_tl
{
	t_rt_kv			*kv;
	t_rt_consumer	*consumer;
	t_state			*state;
}	t_rt_consumer_tl;

void		print_consumer_err(t_rt_consumer *consumer);
bool		process_file(char *fpath, t_state *state);
bool		check_unused(t_rt_consumer *consumer, t_rt_node nd);

// GETTERS:

// 0 nothing
// 1 ok
// 2 diff type
int			get_tl_typed(t_rt_consumer_tl *tl, char *name,
				enum e_RT_NODE_TYPE t, t_rt_node *ret);

t_fvec3		get_fvec3(t_rt_token tk);
float		get_float(t_rt_token tk);
t_dyn_str	get_string(t_rt_consumer *consumer, t_rt_token tk);
bool		get_bool(t_rt_token tk);
t_fvec2		get_fvec2(t_rt_token tk);
int			push_color(t_rt_token tk, t_state *state, bool clamp);
int			push_blackbody(t_rt_token tk, t_state *state);

// ITEMS:
bool		process_cylinder(t_rt_consumer_tl *tl);
bool		process_obj(t_rt_consumer_tl *tl);
bool		process_plane(t_rt_consumer_tl *tl);
bool		process_sphere(t_rt_consumer_tl *tl);
bool		process_ambiant(t_rt_consumer_tl *tl);
bool		process_light(t_rt_consumer_tl *tl, bool is_blackbody);
bool		process_inf_light(t_rt_consumer_tl *tl, bool is_blackbody);
bool		process_sky(t_rt_consumer_tl *tl);
bool		process_camera(t_rt_consumer_tl *tl);

// UTILS:

bool		check_unused(t_rt_consumer *consumer, t_rt_node nd);
bool		check_range(t_rt_consumer *consumer, t_rt_node nd,
				float min, float max);
#endif
