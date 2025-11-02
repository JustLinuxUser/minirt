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

#pragma once

#include "ray.h"
#include "cie.h"
#include "minirt.h"
#include "samplers.h"
#include "shapes.h"
#include "spectrum.h"
#include <math.h>
#include <stdlib.h>

t_collision			collide_ray_slow(t_state *state, t_ray_isector isect);

t_SampledSpectrum	sample_densely_sampled_spectrum(
						const t_densely_sampled_spectrum *spec,
						t_SampledWavelengths lambdas);

t_SampledSpectrum	get_surface_color(t_state *state,
						t_SampledWavelengths lambdas, t_collision coll);

t_SampledSpectrum	sample_sky(t_state *state, t_SampledWavelengths lambdas);
