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

typedef struct {
    float p;
    float q;
    int alias;
} t_Bin;

typedef struct {
    t_Bin bins[3];
} t_AliasTable;


enum LIGHT_TYPE {
    POINT_LIGHT, //PHI = 4 * PI * INTENSITY * SPEC->SAMPLE(LAMBDA)
    DISTANT_LIGHT
};

typedef struct t_DenselySampledSpectrum {
    float samples[471];
} t_DenselySampledSpectrum;

//uniform light sample - vector of lights
typedef struct t_light {
    enum LIGHT_TYPE t;
    float intensity;
    t_fvec3 position;
    t_DenselySampledSpectrum spec;
} t_light;

typedef struct t_lights {
    t_light lights[3];
    int n_lights;
    float total_power;
    float pdfs[3]; //Init to light[i].power/totalPower
    t_AliasTable table;
} t_lights;

typedef struct t_SampledLight {
    t_light light;
    float p;
} t_SampledLight;

t_DenselySampledSpectrum calculateDenselySampledSpectrum(float T);
void add_light(t_lights *lights, t_light to_add);
void calculatePDFs(t_lights *lights);

#endif
