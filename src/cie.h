/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cie.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 10:59:46 by mhornero          #+#    #+#             */
/*   Updated: 2025/07/31 10:44:38 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CIE_H
# define CIE_H

# define CIE_MIN_LAMBDA 360.0
# define CIE_MAX_LAMBDA 830.0
# define CIE_SAMPLES 471 
# define CIE_STEP 1
# define CIE_Y_INTEGRAL 106.856895

extern const float	g_cie_x[CIE_SAMPLES];
extern const float	g_cie_y[CIE_SAMPLES];
extern const float	g_cie_z[CIE_SAMPLES];
extern const float	g_cie_lambda[CIE_SAMPLES];

#endif
