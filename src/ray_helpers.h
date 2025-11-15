/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_helpers.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:22:14 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 20:32:52 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_HELPERS_H
# define RAY_HELPERS_H
# include "ray.h"
# include "minirt.h"
# include "shapes.h"
# include "spectrum.h"

t_collision			collide_ray_slow(t_state *state, t_ray_isector isect);

t_sampled_spec		sample_densely_sampled_spectrum(
						const t_densely_sampled_spectrum *spec,
						t_sampled_lambdas lambdas);

t_sampled_spec		get_surface_color(t_state *state,
						t_sampled_lambdas lambdas, t_collision coll);

t_sampled_spec		sample_sky(t_state *state, t_sampled_lambdas lambdas);
#endif
