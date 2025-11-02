/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 09:51:59 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 09:51:59 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_tokenizer.h"
#include "../libft/libft.h"

void	tokenize_ident_or_bool(t_rt_tokenizer *tokenizer, t_rt_token *ret)
{
	ft_assert(ft_isalpha(peek_char(tokenizer)));
	while (ft_isalpha(peek_char(tokenizer)) || peek_char(tokenizer) == '_')
		consume_char(tokenizer);
	ret->t = RT_IDENT;
	if (str_slice_eq_str(tokenizer->file.contents.buff + ret->start_idx,
			tokenizer->i - ret->start_idx, "true"))
	{
		ret->vals_f[0] = 1;
		ret->t = RT_BOOL;
	}
	if (str_slice_eq_str(tokenizer->file.contents.buff + ret->start_idx,
			tokenizer->i - ret->start_idx, "false"))
	{
		ret->vals_f[0] = 0;
		ret->t = RT_BOOL;
	}
}

void	tokenize_op(t_rt_tokenizer *tokenizer, t_rt_token *ret)
{
	ret->start_idx = tokenizer->i;
	if (peek_char(tokenizer) == '[')
		ret->t = RT_LBRACKET;
	else if (peek_char(tokenizer) == ']')
		ret->t = RT_RBRACKET;
	else if (peek_char(tokenizer) == '{')
		ret->t = RT_LBRACE;
	else if (peek_char(tokenizer) == '}')
		ret->t = RT_RBRACE;
	else if (peek_char(tokenizer) == ':')
		ret->t = RT_COLON;
	else
		ft_assert("Unreachable" == 0);
	consume_char(tokenizer);
}

bool	tokenize_tuple(t_rt_tokenizer *tokenizer, t_rt_token *ret)
{
	bool	is_int;

	ret->is_int = 1;
	ret->t = RT_TUPLE;
	ret->tuple_len = 0;
	ft_assert(ft_strchr("-0123456789", peek_char(tokenizer)) != 0);
	while (ft_strchr("\t\n ", peek_char(tokenizer)) == 0)
	{
		if (!tokenize_number(tokenizer, ret->vals_f + ret->tuple_len, &is_int))
		{
			return (false);
		}
		ret->is_int = ret->is_int && is_int;
		ret->tuple_len++;
		if (peek_char(tokenizer) == ',')
		{
			if (ret->tuple_len == 3)
			{
				tokenizer->err = RT_ERR_TUPLE_TOO_LARGE;
				return (false);
			}
			consume_char(tokenizer);
		}
	}
	return (true);
}

bool	tokenize_string(t_rt_tokenizer *tokenizer, t_rt_token *ret)
{
	ft_assert(consume_char(tokenizer) == '"');
	ret->t = RT_STRING;
	while (peek_char(tokenizer)
		&& peek_char(tokenizer) != '"'
		&& peek_char(tokenizer) != '\n')
		consume_char(tokenizer);
	if (peek_char(tokenizer) != '"')
	{
		tokenizer->err = RT_ERR_UNCLOSED_QUOTE;
		return (false);
	}
	consume_char(tokenizer);
	return (true);
}

bool	next_token(t_rt_tokenizer *tokenizer, t_rt_token *ret)
{
	skip_whitespace_and_comments(tokenizer);
	ret->start_idx = tokenizer->i;
	tokenizer->err_ref = tokenizer->i;
	if (ft_isalpha(peek_char(tokenizer)))
		tokenize_ident_or_bool(tokenizer, ret);
	else if (ft_strchr("[]{}:", peek_char(tokenizer)))
		tokenize_op(tokenizer, ret);
	else if (ft_strchr("-0123456789", peek_char(tokenizer)))
	{
		if (!tokenize_tuple(tokenizer, ret))
			return (false);
	}
	else if (peek_char(tokenizer) == '"')
	{
		if (!tokenize_string(tokenizer, ret))
			return (false);
	}
	else if (peek_char(tokenizer) == 0)
		ret->t = RT_EOF;
	else
		return (tokenizer->err = RT_ERR_UNKNOWN_CHAR, false);
	ret->len = tokenizer->i - ret->start_idx;
	return (true);
}
