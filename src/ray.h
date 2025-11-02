/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 19:56:51 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 20:21:59 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAY_H
# define RAY_H
# include "mymath.h"

typedef struct t_ray_isector
{
	t_ray	ray;
	float	t_min;
	float	t_max;
	void	*ignore_shape;
}	t_ray_isector;

#endif
