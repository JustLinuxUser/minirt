/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light_structs.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 18:50:36 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 18:52:17 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LIGHT_STRUCTS_H
# define LIGHT_STRUCTS_H

# include "mymath.h"

typedef struct s_alias_bin
{
	float	prob;
	int		alias;
}	t_alias_bin;

enum e_LIGHT_TYPE
{
	LIGHT_NONE,
	POINT_LIGHT,
	DISTANT_LIGHT,
	AMBIANT_LIGHT,
};

typedef struct t_densely_sampled_spectrum
{
	float	samples[471];
}	t_densely_sampled_spectrum;

//uniform light sample - vector of lights
typedef struct t_light
{
	enum e_LIGHT_TYPE	t;
	float				intensity;
	t_fvec3				position;
	int					spec_idx;
}	t_light;
#endif
