/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_ends_with.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 13:16:39 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/22 13:16:39 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

bool	str_ends_with(const char *haystack, const char *needle)
{
	const char	*haystack_start;
	const char	*needle_start;

	haystack_start = haystack;
	needle_start = needle;
	while (*haystack)
		haystack++;
	while (*needle)
		needle++;
	while (haystack_start < haystack
		&& needle_start < needle && *haystack == *needle)
	{
		haystack--;
		needle--;
	}
	if (*haystack != *needle
		|| (haystack_start == haystack && needle_start != needle))
		return (false);
	return (true);
}
