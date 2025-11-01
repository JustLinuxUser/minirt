/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_tokenizer_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 18:07:50 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/01 18:14:08 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_tokenizer.h"
#include "../libft/libft.h"

//CHECK CHARS LEFT
int	chars_left(t_obj_tokenizer *tokenizer)
{
	if (tokenizer->curr_idx < tokenizer->str.len)
		return (1);
	return (0);
}

void	init_tok(t_obj_token *tok, enum e_OBJ_TT t, int curr_idx)
{
	*tok = (t_obj_token){0};
	tok->start_idx = curr_idx;
	tok->t = t;
	ft_memset(tok->vals, 0, sizeof(tok->vals));
}

char	consume_obj_char(t_obj_tokenizer *tokenizer)
{
	char	ret;

	ret = tokenizer->str.buff[tokenizer->curr_idx];
	tokenizer->curr_idx++;
	return (ret);
}

char	peek_obj_char(t_obj_tokenizer *tokenizer)
{
	if (tokenizer->str.len == 0)
		return (0);
	if (!chars_left(tokenizer))
		return (0);
	return (tokenizer->str.buff[tokenizer->curr_idx]);
}

char	*obj_type_conversion(unsigned int t)
{
	if (t == OBJ_NONE)
		return ("none");
	else if (t == OBJ_IDENT)
		return ("ident");
	else if (t == OBJ_FLOAT)
		return ("number");
	else if (t == OBJ_COMMENT)
		return ("comment");
	else if (t == OBJ_WHITESPACE)
		return ("whitespace");
	else if (t == OBJ_NEWLINE)
		return ("newline");
	else if (t == OBJ_I_TUPLE)
		return ("tuple");
	else
		return ("eof");
}
