/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   highlight_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 08:48:00 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 08:48:00 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "errors.h"

void	idx_to_row_col(char *buff, int idx, int *row, int *col)
{
	int	r;
	int	i;
	int	c;

	r = 1;
	c = 1;
	i = 0;
	while (i < idx && buff)
	{
		c++;
		if (buff[i] == '\n')
		{
			c = 1;
			r++;
		}
		i++;
	}
	if (row)
		*row = r;
	if (col)
		*col = c;
}

int	row_col_to_idx(char *buff, int row, int col)
{
	int	r;
	int	c;
	int	i;

	if (row <= 0)
		row = 1;
	if (col <= 0)
		col = 1;
	r = 1;
	c = 1;
	i = 0;
	while ((r < row || c < col) && buff)
	{
		c++;
		if (buff[i] == '\n')
		{
			c = 1;
			r++;
		}
		i++;
	}
	return (i);
}

char	*warn_level_to_string(enum e_RT_WARN_LEVEL w)
{
	if (w == RT_HELP)
		return ("HELP");
	if (w == RT_INFO)
		return ("INFO");
	if (w == RT_WARN)
		return ("WARN");
	if (w == RT_ERROR)
		return ("ERROR");
	ft_assert("Unreachable" == 0);
	return (0);
}
