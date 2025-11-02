/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 09:36:02 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/02 09:36:02 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_tokenizer.h"
#include "../libft/utils/utils.h"

char	consume_char(t_rt_tokenizer *tokenizer)
{
	char	ret;

	ret = tokenizer->file.contents.buff[tokenizer->i];
	ft_assert(ret);
	tokenizer->i++;
	return (ret);
}

char	peek_char(t_rt_tokenizer *tokenizer)
{
	if (tokenizer->file.contents.len == 0)
		return (0);
	return (tokenizer->file.contents.buff[tokenizer->i]);
}

void	skip_whitespace(t_rt_tokenizer *tokenizer)
{
	char	c;

	while (1)
	{
		c = peek_char(tokenizer);
		if (c != ' ' && c != '\t' && c != '\n')
			break ;
		consume_char(tokenizer);
	}
}

void	skip_comment(t_rt_tokenizer *tokenizer)
{
	ft_assert(consume_char(tokenizer) == '#');
	while (peek_char(tokenizer) != '\n')
		consume_char(tokenizer);
}

void	skip_whitespace_and_comments(t_rt_tokenizer *tokenizer)
{
	skip_whitespace(tokenizer);
	while (peek_char(tokenizer) == '#')
	{
		skip_comment(tokenizer);
		skip_whitespace(tokenizer);
	}
}
