/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mymath4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 16:38:35 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/05 16:38:35 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mymath.h"

void	sort2f(float *a, float *b)
{
	float	temp;

	if (*a <= *b)
		return ;
	temp = *a;
	*a = *b;
	*b = temp;
}

float	fclamp(float x, float min, float max)
{
	if (x < min)
		return (min);
	else if (x > max)
		return (max);
	else
		return (x);
}
