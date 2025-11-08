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

float	*cie_x(void);
float	*cie_y(void);
float	*cie_z(void);

void	load_cie_x_1(float *ret);
void	load_cie_x_2(float *ret);
void	load_cie_x_3(float *ret);
void	load_cie_x_4(float *ret);
void	load_cie_x_5(float *ret);
void	load_cie_x_6(float *ret);
void	load_cie_x_7(float *ret);
void	load_cie_x_8(float *ret);
void	load_cie_x_9(float *ret);
void	load_cie_x_10(float *ret);
void	load_cie_x_11(float *ret);
void	load_cie_x_12(float *ret);
void	load_cie_x_13(float *ret);
void	load_cie_x_14(float *ret);
void	load_cie_x_15(float *ret);
void	load_cie_x_16(float *ret);
void	load_cie_x_17(float *ret);
void	load_cie_x_18(float *ret);
void	load_cie_x_19(float *ret);
void	load_cie_x_20(float *ret);

void	load_cie_y_1(float *ret);
void	load_cie_y_2(float *ret);
void	load_cie_y_3(float *ret);
void	load_cie_y_4(float *ret);
void	load_cie_y_5(float *ret);
void	load_cie_y_6(float *ret);
void	load_cie_y_7(float *ret);
void	load_cie_y_8(float *ret);
void	load_cie_y_9(float *ret);
void	load_cie_y_10(float *ret);
void	load_cie_y_11(float *ret);
void	load_cie_y_12(float *ret);
void	load_cie_y_13(float *ret);
void	load_cie_y_14(float *ret);
void	load_cie_y_15(float *ret);
void	load_cie_y_16(float *ret);
void	load_cie_y_17(float *ret);
void	load_cie_y_18(float *ret);
void	load_cie_y_19(float *ret);
void	load_cie_y_20(float *ret);

void	load_cie_z_1(float *ret);
void	load_cie_z_2(float *ret);
void	load_cie_z_3(float *ret);
void	load_cie_z_4(float *ret);
void	load_cie_z_5(float *ret);
void	load_cie_z_6(float *ret);
void	load_cie_z_7(float *ret);
void	load_cie_z_8(float *ret);
void	load_cie_z_9(float *ret);
void	load_cie_z_10(float *ret);
void	load_cie_z_11(float *ret);
void	load_cie_z_12(float *ret);
void	load_cie_z_13(float *ret);
void	load_cie_z_14(float *ret);
void	load_cie_z_15(float *ret);
void	load_cie_z_16(float *ret);
void	load_cie_z_17(float *ret);
void	load_cie_z_18(float *ret);
void	load_cie_z_19(float *ret);
void	load_cie_z_20(float *ret);
#endif
