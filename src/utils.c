/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:55:24 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 20:55:50 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "rt_utils.h"

void	free_zero(void *ptr)
{
	void	**real_ptr;

	real_ptr = (void **)ptr;
	free(*real_ptr);
	*real_ptr = 0;
}
