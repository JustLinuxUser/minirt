/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 00:13:26 by anddokhn          #+#    #+#             */
/*   Updated: 2024/10/14 18:53:14 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../libft.h"

static int	find_block(int *end, char *str, char c)
{
	int	i;
	int	prev;

	i = 0;
	prev = -1;
	if (ft_strlen(str) == 0)
		return (-1);
	while ((size_t)i <= ft_strlen(str))
	{
		if (c == str[i] || !str[i])
		{
			*end = i;
			if ((i - prev) > 1)
				return (prev + 1);
			prev = i;
		}
		i++;
	}
	return (-1);
}

static int	num_blocks(char *str, char c)
{
	int	i;
	int	occ;
	int	end;
	int	start;

	i = 0;
	occ = 0;
	while (str[i] != 0)
	{
		start = find_block(&end, str + i, c);
		if (start == -1)
			return (occ);
		occ++;
		i += end;
	}
	return (occ);
}

static void	*free_list(char **list, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(list[i]);
		i++;
	}
	free(list);
	return (0);
}

char	**ft_split(char *str, char c)
{
	char	**out;
	int		i;
	int		start;
	int		occ;
	int		end;

	out = ft_calloc((num_blocks(str, c) + 1), sizeof(char *));
	if (out == 0)
		return (0);
	i = 0;
	occ = 0;
	while (str[i] != 0)
	{
		start = find_block(&end, str + i, c);
		if (start == -1)
			return (out);
		out[occ] = malloc(end - start + 1);
		if (out[occ] == 0)
			return (free_list(out, occ));
		ft_strlcpy(out[occ++], str + i + start, end - start + 1);
		i += end;
	}
	return (out);
}
