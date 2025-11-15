/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:15:32 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 16:15:32 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DRAW_H
# define DRAW_H
# include "minirt.h"

void			draw(t_state *state);
void			*render_step(void *arg);
int				coord_to_idx(t_state *state, int x, int y);
void			idx_to_coords(t_state *state, size_t idx, int *x, int *y);
t_rand_state	move_tl_prng(t_state *state);
void			finish_render_loop(t_state *state);

int				thread_idx(t_state *state, int mask);
void			emergency_exit(t_state *state);
void			render_multithread(t_state *state);
#endif
