/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 09:16:04 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 09:16:04 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_parser.h"

bool	parse_leaf(t_rt_parser *parser, t_rt_node *ret)
{
	ft_assert(is_leaf(peek_token_type(&parser->tokenizer)));
	*ret = (t_rt_node){.token = consume_token_panic(&parser->tokenizer)};
	if (ret->token.t == RT_STRING)
		ret->t = RT_ND_STRING;
	if (ret->token.t == RT_TUPLE)
		ret->t = tuple_token_type(ret->token);
	if (ret->token.t == RT_BOOL)
		ret->t = RT_ND_BOOL;
	return (true);
}

bool	parse_node(t_rt_parser *parser, t_rt_node *ret)
{
	if (!is_rvalue(peek_token_type(&parser->tokenizer)))
		return (unexpected_err(parser));
	if (is_leaf(peek_token_type(&parser->tokenizer)))
		return (parse_leaf(parser, ret));
	if (peek_token_type(&parser->tokenizer) == RT_LBRACE)
		return (parse_dict(parser, ret));
	if (peek_token_type(&parser->tokenizer) == RT_LBRACKET)
		return (parse_list(parser, ret));
	return (unexpected_err(parser));
}

bool	parse_list(t_rt_parser *parser, t_rt_node *ret)
{
	t_rt_node	nd;

	*ret = (t_rt_node){.t = RT_ND_LIST};
	ft_assert(peek_token_type(&parser->tokenizer) == RT_LBRACKET);
	ret->token = consume_token_panic(&parser->tokenizer);
	while (is_rvalue(peek_token_type(&parser->tokenizer)))
	{
		if (!parse_node(parser, &nd))
		{
			vec_rt_nd_push(&ret->list, nd);
			return (false);
		}
		vec_rt_nd_push(&ret->list, nd);
	}
	if (peek_token_type(&parser->tokenizer) != RT_RBRACKET)
		return (unexpected_err(parser));
	consume_token_panic(&parser->tokenizer);
	return (true);
}

// until the next top level ident
bool	parse_old_array(t_rt_parser *parser, t_rt_node *ret)
{
	t_rt_token	t;

	*ret = (t_rt_node){.t = RT_ND_LIST};
	while (is_rvalue(peek_token_type(&parser->tokenizer)))
	{
		if (peek_token_type(&parser->tokenizer) == RT_NONE)
			return (false);
		if (peek_token_type(&parser->tokenizer) != RT_TUPLE)
		{
			parser->err = RT_ERR_NEW_SYNTAX_IN_OLD_CONTEXT;
			return (false);
		}
		t = consume_token_panic(&parser->tokenizer);
		vec_rt_nd_push(&ret->list,
			(t_rt_node){.t = tuple_token_type(t), .token = t});
	}
	return (true);
}

bool	parse_statement(t_rt_parser *parser, t_rt_kv *stmt)
{
	t_rt_token	tk;

	*stmt = (t_rt_kv){0};
	if (peek_token_type(&parser->tokenizer) != RT_IDENT)
		return (unexpected_err(parser));
	consume_token(&parser->tokenizer, &stmt->k);
	if (peek_token_type(&parser->tokenizer) == RT_COLON)
	{
		consume_token(&parser->tokenizer, &tk);
		if (peek_token_type(&parser->tokenizer) != RT_LBRACE)
			return (unexpected_err(parser));
		return (parse_dict(parser, &stmt->v));
	}
	else
		return (parse_old_array(parser, &stmt->v));
	return (true);
}
