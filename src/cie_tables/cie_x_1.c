/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out_x.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 10:41:47 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/08 10:41:47 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cie.h"
#include <stdbool.h>

float	*cie_x(void)
{
	static float	ret[CIE_SAMPLES];
	static bool		loaded = false;

	if (!loaded)
		load_cie_x_1(ret);
	if (!loaded)
		loaded = true;
	return (ret);
}
