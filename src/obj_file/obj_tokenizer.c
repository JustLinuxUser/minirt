/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_tokenizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:10:19 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/01 19:44:14 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_tokenizer.h"
#include "../libft/libft.h"

int	init_tokenizer(char *filename, t_obj_tokenizer *tokenizer, t_dyn_str *ret)
{
	*ret = (t_dyn_str){0};
	*tokenizer = (t_obj_tokenizer){0};
	if (!dyn_str_read_file(filename, ret))
		return (0);
	tokenizer->str = *ret;
	return (1);
}

void	tokenize_comm_ws_nl(t_obj_tokenizer *tokenizer)
{
	if (peek_obj_char(tokenizer) == '#')
		tokenize_comment(tokenizer);
	else if (peek_obj_char(tokenizer) == '\n'
		|| peek_obj_char(tokenizer) == '\r')
		tokenize_newline(tokenizer);
	else if (peek_obj_char(tokenizer) == ' ')
		consume_obj_char(tokenizer);
}

void	tokenizer_tuple_error(t_obj_tokenizer *tokenizer)
{
	free(tokenizer->tokens.buff);
	free(tokenizer->str.buff);
	*tokenizer = (t_obj_tokenizer){0};
}

//add token array to function signature
int	tokenize_obj(char *filename, t_obj_tokenizer *tokenizer)
{
	t_dyn_str	ret;

	if (!init_tokenizer(filename, tokenizer, &ret))
		return (0);
	while (chars_left(tokenizer))
	{
		if (peek_obj_char(tokenizer) == '#' || peek_obj_char(tokenizer) == '\n'
			|| peek_obj_char(tokenizer) == '\r'
			|| peek_obj_char(tokenizer) == ' ')
			tokenize_comm_ws_nl(tokenizer);
		else if (peek_obj_char(tokenizer) == '-'
			|| peek_obj_char(tokenizer) == '/'
			|| ft_strchr("0123456789", peek_obj_char(tokenizer)))
		{
			if (!tokenize_obj_tuple(tokenizer))
			{
				tokenizer_tuple_error(tokenizer);
				return (0);
			}
		}
		else
			tokenize_word(tokenizer);
	}
	return (1);
}
