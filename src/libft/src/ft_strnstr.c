/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 18:27:28 by anddokhn          #+#    #+#             */
/*   Updated: 2024/09/17 18:27:29 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	int		offset;

	if ((str[0] == 0 || len == 0) && to_find[0] == 0)
		return ((char *)str);
	i = 0;
	while (str[i] != 0 && i < len)
	{
		offset = 0;
		while (str[i + offset] == to_find[offset] && to_find[offset] != 0
			&& str[i + offset] != 0 && i + offset < len)
			offset++;
		if (to_find[offset] == 0)
			return ((char *)&str[i]);
		i++;
	}
	return (0);
}
