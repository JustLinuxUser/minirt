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

void	print_consumer_err(t_rt_consumer *consumer);
bool	process_file(char *fpath, t_state *state);
#endif
