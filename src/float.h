/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:16:19 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 16:16:19 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FLOAT_H
# define FLOAT_H

# define MACHINE_EPSILON 0x1p-24

inline static float	f32_gamma(int n)
{
	return ((n * MACHINE_EPSILON) / (1 - n * MACHINE_EPSILON));
}
#endif
