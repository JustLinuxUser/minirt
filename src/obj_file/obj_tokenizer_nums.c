/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_tokenizer_nums.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 18:20:13 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/01 19:08:08 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_tokenizer.h"
#include "../libft/libft.h"
#include <math.h>

int	tokenize_whole_part(t_obj_tokenizer *tokenizer,
		t_obj_token *tok, bool *is_int)
{
	int	whole_part;

	whole_part = 0;
	while (ft_strchr("0123456789", peek_obj_char(tokenizer)))
	{
		whole_part *= 10;
		whole_part += consume_obj_char(tokenizer) - '0';
		tok->len++;
	}
	if (peek_obj_char(tokenizer) == '.')
	{
		consume_obj_char(tokenizer);
		tok->len++;
		*is_int = false;
	}
	return (whole_part);
}

int	tokenize_fract_part(t_obj_tokenizer *tokenizer,
		t_obj_token *tok, int *num_fract)
{
	int	fract_part;

	fract_part = 0;
	*num_fract = 0;
	while (ft_strchr("0123456789", peek_obj_char(tokenizer)))
	{
		fract_part *= 10;
		fract_part += consume_obj_char(tokenizer) - '0';
		tok->len++;
		(*num_fract)++;
	}
	return (fract_part);
}

bool	tokenize_obj_number(t_obj_tokenizer *tokenizer,
		float *fret, bool *is_int, t_obj_token *tok)
{
	int	whole_part;
	int	fract_part;
	int	num_fract;
	int	sign;

	ft_assert(ft_strchr("-0123456789", peek_obj_char(tokenizer)) != 0);
	*is_int = true;
	sign = 1;
	if (peek_obj_char(tokenizer) == '-')
	{
		sign = -1;
		consume_obj_char(tokenizer);
		tok->len++;
	}
	whole_part = tokenize_whole_part(tokenizer, tok, is_int);
	num_fract = 0;
	fract_part = tokenize_fract_part(tokenizer, tok, &num_fract);
	*fret = whole_part * sign;
	*fret += sign * fract_part / powf(10, num_fract);
	return (true);
}
