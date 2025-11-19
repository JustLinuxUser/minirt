/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:16:19 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/19 17:34:11 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FLOAT_UTILS_H
# define FLOAT_UTILS_H

# define MACHINE_EPSILON 0.000000059604644775390625
# define FLT_EPSILON 1.19209290e-7F

inline static float	f32_gamma(int n)
{
	return ((n * MACHINE_EPSILON) / (1 - n * MACHINE_EPSILON));
}
#endif
