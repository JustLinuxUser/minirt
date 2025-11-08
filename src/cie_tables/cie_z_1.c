/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cie_z_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 11:45:03 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/08 11:45:03 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cie.h"
#include <stdbool.h>

float	*cie_z(void)
{
	static float	ret[CIE_SAMPLES];
	static bool		loaded = false;

	if (!loaded)
		load_cie_z_1(ret);
	if (!loaded)
		loaded = true;
	return (ret);
}
