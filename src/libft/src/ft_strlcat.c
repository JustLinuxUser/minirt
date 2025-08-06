/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:25:11 by anddokhn          #+#    #+#             */
/*   Updated: 2024/10/14 18:54:07 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	src_len;
	size_t	dest_init_len;
	size_t	i;

	src_len = ft_strlen(src);
	dest_init_len = 0;
	while (dest[dest_init_len] != 0 && (unsigned int)dest_init_len < size)
		dest_init_len++;
	i = 0;
	while (dest_init_len + i + 1 < size && i < src_len)
	{
		dest[dest_init_len + i] = src[i];
		i++;
	}
	if (dest_init_len < size)
		dest[dest_init_len + i] = 0;
	return (src_len + dest_init_len);
}
