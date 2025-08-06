/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:59:04 by anddokhn          #+#    #+#             */
/*   Updated: 2025/03/16 05:28:13 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*ret;

	len = ft_strlen(s);
	ret = malloc(len + 1);
	if (ret == 0)
		return (0);
	ft_strlcpy(ret, s, len + 1);
	return (ret);
}

char	*ft_strndup(const char *s, int len)
{
	char	*ret;

	ret = malloc(len + 1);
	if (ret == 0)
		return (0);
	ft_strlcpy(ret, s, len + 1);
	return (ret);
}
