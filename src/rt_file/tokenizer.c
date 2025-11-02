/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 19:29:40 by anddokhn          #+#    #+#             */
/*   Updated: 2025/11/01 19:29:40 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt_tokenizer.h"
#include <stdbool.h>
#include "../libft/utils/utils.h"

bool	consume_token(t_rt_tokenizer *tokenizer, t_rt_token *ret)
{
	if (tokenizer->has_token)
	{
		if (tokenizer->err != RT_ERR_NONE)
			return (false);
		*ret = tokenizer->curr_tok;
		tokenizer->has_token = false;
		return (true);
	}
	if (next_token(tokenizer, ret))
		return (true);
	return (false);
}

t_rt_token	consume_token_panic(t_rt_tokenizer *tokenizer)
{
	t_rt_token	ret;

	ft_assert(consume_token(tokenizer, &ret));
	return (ret);
}

bool	peek_token(t_rt_tokenizer *tokenizer, t_rt_token *ret)
{
	if (tokenizer->err != RT_ERR_NONE)
		return (RT_NONE);
	if (tokenizer->has_token)
	{
		*ret = tokenizer->curr_tok;
		return (true);
	}
	if (next_token(tokenizer, &tokenizer->curr_tok))
	{
		tokenizer->has_token = 1;
		*ret = tokenizer->curr_tok;
		return (true);
	}
	return (false);
}

enum e_RT_TT	peek_token_type(t_rt_tokenizer *tokenizer)
{
	if (tokenizer->err != RT_ERR_NONE)
		return (RT_NONE);
	if (tokenizer->has_token)
		return (tokenizer->curr_tok.t);
	if (next_token(tokenizer, &tokenizer->curr_tok))
	{
		tokenizer->has_token = 1;
		return (tokenizer->curr_tok.t);
	}
	return (RT_NONE);
}
