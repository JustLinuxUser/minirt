#include "rt_tokenizer.h"
#include <math.h>
#include <stdbool.h>
#include "../libft/libft.h"

char	consume_char(t_rt_tokenizer *tokenizer) {
	char ret = tokenizer->file.buff[tokenizer->i];

	ft_assert(ret);
	tokenizer->i++;
	return (ret);
}

char peek_char(t_rt_tokenizer *tokenizer) {
	if (tokenizer->file.len == 0)
		return (0);
	return (tokenizer->file.buff[tokenizer->i]);
}

void skip_whitespace(t_rt_tokenizer *tokenizer) {
	char c;

	while (1) {
		c = peek_char(tokenizer);
		if (c != ' ' && c != '\t' && c != '\n')
			break;
		consume_char(tokenizer);
	}
}

void	skip_comment(t_rt_tokenizer *tokenizer) {
	ft_assert(consume_char(tokenizer) == '#');
	while (peek_char(tokenizer) != '\n')
		consume_char(tokenizer);
}
void skip_whitespace_and_comments(t_rt_tokenizer *tokenizer) {
	skip_whitespace(tokenizer);
	while (peek_char(tokenizer) == '#')
	{
		skip_comment(tokenizer);
		skip_whitespace(tokenizer);
	}
}

void	tokenize_ident(t_rt_tokenizer *tokenizer, t_rt_token *ret) {
	ft_assert(ft_isalpha(peek_char(tokenizer)));
	while (ft_isalpha(peek_char(tokenizer)) || peek_char(tokenizer) == '_')
		consume_char(tokenizer);
	ret->t = RT_IDENT;
}

void tokenize_op(t_rt_tokenizer *tokenizer, t_rt_token *ret) {
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

bool tokenize_number(t_rt_tokenizer *tokenizer, float *fret, bool *is_int) {
	int whole_part;
	int fract_part;
	int num_fract;
	int sign;

	ft_assert(ft_strchr("-0123456789", peek_char(tokenizer)) != 0);
	*is_int = true;
	whole_part = 0;
	sign = 1;
	if (peek_char(tokenizer) == '-') {
		sign = -1;
		consume_char(tokenizer);
	}
	if (!ft_strchr("0123456789", peek_char(tokenizer)))
	{
		tokenizer->err = RT_ERR_INVALID_NUM;
		return (false);
	}
	while (ft_strchr("0123456789", peek_char(tokenizer)))
	{
		whole_part *= 10;
		whole_part += consume_char(tokenizer) - '0';
	}
	if (peek_char(tokenizer) == '.')
	{
		consume_char(tokenizer);
		*is_int = false;
	}
	fract_part = 0;
	num_fract = 0;
	while (ft_strchr("0123456789", peek_char(tokenizer)))
	{
		fract_part *= 10;
		fract_part += consume_char(tokenizer) - '0';
		num_fract++;
	}
	*fret = whole_part * sign;
	*fret += fract_part / powf(10, num_fract);
	return (true);
}

bool tokenize_tuple(t_rt_tokenizer *tokenizer, t_rt_token *ret) {
	bool	is_int;

	ret->is_int = 1;
	ret->t = RT_TUPLE;
	ret->tuple_len = 0;

	ft_assert(ft_strchr("-0123456789", peek_char(tokenizer)) != 0);
	while (ft_strchr("-0123456789", peek_char(tokenizer)) != 0) {
		if (!tokenize_number(tokenizer, ret->vals_f + ret->tuple_len, &is_int)) {
			return (false);
		}
		ret->is_int = ret->is_int && is_int;
		ret->tuple_len++;
		if (peek_char(tokenizer) == ',')
		{
			consume_char(tokenizer);
			if (ret->tuple_len == 3)
			{
				tokenizer->err = RT_ERR_TUPLE_TOO_LARGE;
				return (false);
			}
		}
	}
	return (true);
}

bool	tokenize_string(t_rt_tokenizer *tokenizer, t_rt_token *ret) {
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

bool next_token(t_rt_tokenizer *tokenizer, t_rt_token *ret) {
	skip_whitespace_and_comments(tokenizer);
	ret->start_idx = tokenizer->i;
	tokenizer->err_ref = tokenizer->i;
	if (ft_isalpha(peek_char(tokenizer)))
		tokenize_ident(tokenizer, ret);
	else if (ft_strchr("[]{}:", peek_char(tokenizer)))
		tokenize_op(tokenizer, ret);
	else if (ft_strchr("-0123456789", peek_char(tokenizer))) {
		if (!tokenize_tuple(tokenizer, ret))
			return (false);
	} else if (peek_char(tokenizer) == '"') {
		if (!tokenize_string(tokenizer, ret))
			return (false);
	} else if (peek_char(tokenizer) == 0) {
		ret->t = RT_EOF;
	} else {
		tokenizer->err = RT_ERR_UNKNOWN_CHAR;
		return false;
	}
	ret->len = tokenizer->i - ret->start_idx;
	return true;
}

bool consume_token(t_rt_tokenizer *tokenizer, t_rt_token *ret)
{
	if (tokenizer->has_token)
	{
		*ret = tokenizer->curr_tok;
		tokenizer->has_token = false;
		return (true);
	}
	if (next_token(tokenizer, ret))
		return (true);
	return (false);
}

t_rt_token consume_token_panic(t_rt_tokenizer *tokenizer)
{
	t_rt_token	ret;

	ft_assert(consume_token(tokenizer, &ret));
	return (ret);
}

bool peek_token(t_rt_tokenizer *tokenizer, t_rt_token *ret) {
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

enum RT_TT peek_token_type(t_rt_tokenizer *tokenizer)
{
	if (tokenizer->has_token)
		return (tokenizer->curr_tok.t);
	if (next_token(tokenizer, &tokenizer->curr_tok))
	{
		tokenizer->has_token = 1;
		return (tokenizer->curr_tok.t);
	}
	return (RT_NONE);
}
