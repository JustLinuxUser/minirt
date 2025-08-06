/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:52:57 by anddokhn          #+#    #+#             */
/*   Updated: 2024/12/28 14:50:59 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../utils/utils.h"

static int	num_len(int n)
{
	int	count;

	count = 0;
	if (n < 0)
		count++;
	count++;
	n /= 10;
	while (n != 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*ret;
	int		i;
	int		len;

	len = num_len(n);
	ret = malloc(len + 1);
	if (ret == 0)
		return (0);
	if (n < 0)
		ret[0] = '-';
	i = len - 1;
	ret[i--] = ft_abs(n % 10) + '0';
	n /= 10;
	while (n != 0)
	{
		ret[i--] = ft_abs(n % 10) + '0';
		n /= 10;
	}
	ret[len] = 0;
	return (ret);
}
