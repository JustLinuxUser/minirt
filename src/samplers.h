/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   samplers.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:35:46 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 20:36:33 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SAMPLERS_H
# define SAMPLERS_H
# include "mymath.h"
# include <stdbool.h>
# include <stdint.h>

typedef struct sampler_state
{
	int	stratified_x;
	int	stratified_y;
	int	last_x;
	int	last_y;
}	t_sampler_state;

bool	sample_stratified(t_sampler_state *state, t_fvec2 *sample);
t_fvec3	rand_halfsphere(t_fvec3 norm, uint64_t *rand_state);
t_fvec3	rand_direction(uint64_t *rand_state);
#endif
