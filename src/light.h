/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:15:54 by mhornero          #+#    #+#             */
/*   Updated: 2025/08/24 17:54:30 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
# define LIGHT_H
#include "mymath.h"


typedef struct {
    float	prob;
    int		alias;
} t_alias_bin;

#include "libft/generated/vec_alias_bin.h"

enum LIGHT_TYPE {
    POINT_LIGHT, //PHI = 4 * PI * INTENSITY * SPEC->SAMPLE(LAMBDA)
    DISTANT_LIGHT
};

typedef struct t_densely_sampled_spectrum {
    float samples[471];
} t_densely_sampled_spectrum;

extern const t_densely_sampled_spectrum SPECTRUM_ONES;
extern const t_densely_sampled_spectrum SPECTRUM_ZEROS;

#include "libft/generated/vec_densely_sampled_spectrum.h"

//uniform light sample - vector of lights
typedef struct t_light {
    enum LIGHT_TYPE t;
    float intensity;
    t_fvec3 position;
    int spec_idx;
} t_light;

#include "libft/generated/vec_light.h"
#include "libft/generated/vec_float.h"

typedef struct t_lights {
    float total_power;

    t_vec_light lights;
    t_vec_float pdfs; //TODO: Init to light[i].power/totalPower
	t_vec_alias_bin bins;
} t_lights;

typedef struct t_SampledLight {
    t_light light;
    float p;
} t_SampledLight;

t_densely_sampled_spectrum calculateDenselySampledSpectrum(float T);
void add_light(t_lights *lights, t_light to_add);
void calculatePDFs(t_lights *lights);

#endif
