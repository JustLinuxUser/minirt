/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 18:03:29 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/01 19:23:33 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_parser.h"
#include "../rt_utils.h"

bool	tokens_left(t_obj_parser *parser)
{
	if (parser->curr_token < (size_t)parser->tokenizer.tokens.len)
		return (1);
	return (0);
}

t_obj_token	consume_obj_token(t_obj_parser *parser)
{
	return (parser->tokenizer.tokens.buff[parser->curr_token++]);
}

t_obj_token	peek_obj_token(t_obj_parser *parser)
{
	return (parser->tokenizer.tokens.buff[parser->curr_token]);
}
