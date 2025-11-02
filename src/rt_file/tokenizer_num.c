/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_num.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 09:48:46 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 09:48:46 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_tokenizer.h"
#include "../libft/libft.h"
#include <math.h>

static int	tokenize_whole_part(t_rt_tokenizer *tokenizer, bool *is_int)
{
	int	whole_part;

	whole_part = 0;
	while (ft_strchr("0123456789", peek_char(tokenizer)))
	{
		whole_part *= 10;
		whole_part += consume_char(tokenizer) - '0';
	}
	if (peek_char(tokenizer) == '.')
	{
		consume_char(tokenizer);
		*is_int = false;
	}
	return (whole_part);
}

static	int	tokenize_fract_part(t_rt_tokenizer *tokenizer,
		int *num_fract)
{
	int	fract_part;

	fract_part = 0;
	*num_fract = 0;
	while (ft_strchr("0123456789", peek_char(tokenizer)))
	{
		fract_part *= 10;
		fract_part += consume_char(tokenizer) - '0';
		(*num_fract)++;
	}
	return (fract_part);
}

bool	tokenize_number(t_rt_tokenizer *tokenizer,
		float *fret, bool *is_int)
{
	int	whole_part;
	int	fract_part;
	int	num_fract;
	int	sign;

	if (ft_strchr("-0123456789", peek_char(tokenizer)) == 0)
		return (false);
	*is_int = true;
	sign = 1;
	if (peek_char(tokenizer) == '-')
	{
		sign = -1;
		consume_char(tokenizer);
	}
	whole_part = tokenize_whole_part(tokenizer, is_int);
	num_fract = 0;
	fract_part = tokenize_fract_part(tokenizer, &num_fract);
	*fret = whole_part * sign;
	*fret += sign * fract_part / powf(10, num_fract);
	return (true);
}
