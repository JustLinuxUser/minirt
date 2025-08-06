/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 18:01:48 by anddokhn          #+#    #+#             */
/*   Updated: 2024/10/14 18:54:12 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdlib.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	string_len;
	size_t	substr_len;
	char	*ret;

	string_len = ft_strlen(s);
	if (start >= string_len)
		substr_len = 0;
	else if (string_len - start > len)
		substr_len = len;
	else
		substr_len = string_len - start;
	ret = ft_calloc(substr_len + 1, 1);
	if (ret == 0)
		return (0);
	if (substr_len)
		ft_strlcpy(ret, s + start, substr_len + 1);
	return (ret);
}
