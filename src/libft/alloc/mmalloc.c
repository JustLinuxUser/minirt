/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmalloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 16:31:01 by anddokhn          #+#    #+#             */
/*   Updated: 2025/08/25 16:31:01 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mmalloc.h"
#include "../libft.h"

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void	*mmalloc(size_t amount, char *msg)
{
	void	*ret;

	ret = malloc(amount);
	if (!ret)
	{
		ft_eprintf("ERROR: Failed to allocate %zu bytes "
			"of memory while: %s, %s\n",
			amount, msg, strerror(errno));
		exit(1);
	}
	ft_memset(ret, 0, amount);
	return (ret);
}
