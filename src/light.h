/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:15:54 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 18:52:04 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIGHT_H
# define LIGHT_H
# include "light_structs.h"
# include "libft/generated/vec_alias_bin.h"
# include "libft/generated/vec_light.h"
# include "libft/generated/vec_float.h"
# include "libft/generated/vec_densely_sampled_spectrum.h"

typedef struct t_lights
{
	float			total_power;
	t_vec_light		lights;
	t_vec_float		pdfs;
	t_vec_alias_bin	bins;
}	t_lights;

typedef struct t_SampledLight
{
	t_light	light;
	float	p;
}	t_SampledLight;

t_densely_sampled_spectrum	densely_sampled_spectrum_blackbody(float T);
void						add_light(t_lights *lights, t_light to_add);
void						calculate_pdfs(t_lights *lights);

#endif
