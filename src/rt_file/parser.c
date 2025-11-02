/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 09:12:17 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 09:12:17 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include "../libft/dsa/dyn_str.h"
#include "rt_parser.h"
#include "rt_tokenizer.h"

bool	try_push_checked_dup(t_rt_parser *parser,
							t_vec_rt_kv *v,
							t_rt_kv kv,
							bool upper_only)
{
	t_rt_kv		*duplicate;
	t_rt_token	dup_tk;
	char		first_char;

	first_char = parser->tokenizer.file.contents.buff[kv.k.start_idx];
	duplicate = vec_rt_kv_get(v, parser->tokenizer.file.contents.buff, kv.k);
	if (duplicate && (!upper_only || (first_char >= 'A' && first_char <= 'Z')))
	{
		dup_tk = duplicate->k;
		vec_rt_kv_push(v, kv);
		return (duplicate_key_err(parser, dup_tk, kv.k));
	}
	vec_rt_kv_push(v, kv);
	return (true);
}

bool	parse_dict(t_rt_parser *parser, t_rt_node *ret)
{
	t_rt_kv	kv;

	kv = (t_rt_kv){0};
	*ret = (t_rt_node){.t = RT_ND_DICT,
		.token = consume_token_panic(&parser->tokenizer)};
	while (peek_token_type(&parser->tokenizer) == RT_IDENT)
	{
		kv.k = consume_token_panic(&parser->tokenizer);
		if (peek_token_type(&parser->tokenizer) != RT_COLON)
			return (unexpected_err(parser));
		consume_token_panic(&parser->tokenizer);
		if (!is_rvalue(peek_token_type(&parser->tokenizer)))
			return (unexpected_err(parser));
		if (!parse_node(parser, &kv.v))
		{
			vec_rt_kv_push(&ret->dict, kv);
			return (false);
		}
		if (!try_push_checked_dup(parser, &ret->dict, kv, false))
			return (false);
	}
	if (peek_token_type(&parser->tokenizer) != RT_RBRACE)
		return (unexpected_err(parser));
	consume_token_panic(&parser->tokenizer);
	return (true);
}

static bool	process_token(t_rt_parser *parser, t_rt_kv *stmt, bool *ret)
{
	if (peek_token_type(&parser->tokenizer) == RT_EOF)
	{
		*ret = true;
		return (false);
	}
	if (!parse_statement(parser, stmt))
	{
		if (parser->tokenizer.err)
			print_tokenizer_err(&parser->tokenizer);
		else
			print_parser_err(parser);
		free_node(stmt->v);
		return (false);
	}
	pprint_node(*parser, stmt->v, 0);
	if (!try_push_checked_dup(parser, &parser->statements, *stmt, true))
	{
		print_parser_err(parser);
		return (false);
	}
	return (true);
}

bool	parse_file(t_rt_parser *parser)
{
	bool	ret;
	t_rt_kv	stmt;

	ret = false;
	while (1)
	{
		if (!process_token(parser, &stmt, &ret))
			break ;
	}
	return (ret);
}
