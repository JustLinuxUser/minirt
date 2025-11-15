/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_structs.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 17:26:57 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/15 17:26:57 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "light_structs.h"
#include "shapes.h"

typedef struct s_light_color_sampler
{
	t_light		light;
	float		light_t;
	t_collision	light_coll;
	int			light_idx;
	t_fvec3		light_dir;
}	t_light_color_sampler;

typedef struct s_integrator
{
	t_sampled_spec		l;
	t_sampled_spec		beta;
	t_collision			coll;
	int					i;
	int					iter;
	t_fvec3				p;
	t_fvec3				norm;
	void				*ignored_shape;
	t_ray				ray;
}	t_integrator;
