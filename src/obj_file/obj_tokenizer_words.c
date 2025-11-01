/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_tokenizer_words.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:42:16 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/01 19:42:40 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_tokenizer.h"
#include "../libft/libft.h"

void	tokenize_comment(t_obj_tokenizer *tokenizer)
{
	t_obj_token	comm;

	init_tok(&comm, OBJ_COMMENT, tokenizer->curr_idx);
	while (consume_obj_char(tokenizer) != '\n')
		comm.len++;
	vec_obj_token_push(&tokenizer->tokens, comm);
}

bool	tokenize_word(t_obj_tokenizer *tokenizer)
{
	t_obj_token	tok;

	init_tok(&tok, OBJ_IDENT, tokenizer->curr_idx);
	while (peek_obj_char(tokenizer) != ' '
		&& peek_obj_char(tokenizer) != '\n' && chars_left(tokenizer))
	{
		tok.len++;
		consume_obj_char(tokenizer);
	}
	vec_obj_token_push(&tokenizer->tokens, tok);
	return (true);
}

void	tokenize_newline(t_obj_tokenizer *tokenizer)
{
	t_obj_token	tok;

	init_tok(&tok, OBJ_NEWLINE, tokenizer->curr_idx);
	while (peek_obj_char(tokenizer) == '\n'
		|| peek_obj_char(tokenizer) == '\r')
	{
		tok.len++;
		tokenizer->curr_idx++;
	}
	vec_obj_token_push(&tokenizer->tokens, tok);
}
