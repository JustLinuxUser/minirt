/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_tokenizer_tuples.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:07:35 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/01 19:22:27 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_tokenizer.h"
#include "../libft/libft.h"
#include <math.h>
#include <stdbool.h>

bool	tokenize_tuple_conditions(t_obj_tokenizer *tokenizer, t_obj_token *tok)
{
	if (tok->num_vals >= 3)
		return (false);
	if (peek_obj_char(tokenizer) == '/')
	{
		consume_obj_char(tokenizer);
		tok->len++;
	}
	return (true);
}

bool	tokenize_tuple_number(t_obj_tokenizer *tokenizer, t_obj_token *tok,
		bool *is_int, float_t *ret)
{
	if (!tokenize_obj_number(tokenizer, ret, is_int, tok))
		return (false);
	tok->parsed_num = *ret;
	tok->vals[tok->num_vals] = *ret;
	return (true);
}

void	init_tuple_variables(t_obj_tokenizer *tokenizer, t_obj_token *tok,
		bool *is_int, float_t *ret)
{
	init_tok(tok, OBJ_I_TUPLE, tokenizer->curr_idx);
	*is_int = true;
	*ret = 0.f;
}

bool	is_float(t_obj_token *tok)
{
	if (tok->num_vals != 0)
		return (false);
	tok->t = OBJ_FLOAT;
	return (true);
}

bool	tokenize_obj_tuple(t_obj_tokenizer *tokenizer)
{
	t_obj_token	tok;
	bool		is_int;
	float_t		ret;

	init_tuple_variables(tokenizer, &tok, &is_int, &ret);
	while (chars_left(tokenizer)
		&& ft_strchr("-0123456789/", peek_obj_char(tokenizer)) != 0)
	{
		if (!tokenize_tuple_conditions(tokenizer, &tok))
			return (false);
		if (ft_strchr("-0123456789", peek_obj_char(tokenizer)))
		{
			if (!tokenize_tuple_number(tokenizer, &tok, &is_int, &ret))
				return (false);
			if (!is_int)
			{
				if (!is_float(&tok))
					return (false);
				break ;
			}
		}
		tok.num_vals++;
	}
	vec_obj_token_push(&tokenizer->tokens, tok);
	return (true);
}
