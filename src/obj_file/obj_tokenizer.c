/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_tokenizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:10:19 by mhornero          #+#    #+#             */
/*   Updated: 2025/09/24 15:53:22 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_tokenizer.h"
#include "../libft/libft.h"

//CHECK CHARS LEFT
int chars_left(t_obj_tokenizer* tokenizer)
{
    if (tokenizer->curr_idx < tokenizer->str.len)
        return 1;
    return 0;
}

void init_tok(t_obj_token* tok, enum OBJ_TT t, int curr_idx)
{
    tok->start_idx = curr_idx;
    tok->t = t;
    tok->len = 0;
    tok->parsed_num = -99999.f;
    ft_memset(tok->vals, 0, sizeof(tok->vals));
}

char consume_obj_char(t_obj_tokenizer* tokenizer)
{
    char ret = tokenizer->str.buff[tokenizer->curr_idx];

	tokenizer->curr_idx++;
	return (ret);
}

char peek_obj_char(t_obj_tokenizer* tokenizer)
{
    if (tokenizer->str.len == 0)
		return (0);
    if (!chars_left(tokenizer))
        return (0);
	return (tokenizer->str.buff[tokenizer->curr_idx]);   
}

void tokenize_comment(t_obj_tokenizer* tokenizer)
{
    t_obj_token comm;
    init_tok(&comm, OBJ_COMMENT, tokenizer->curr_idx);
    while (consume_obj_char(tokenizer) != '\n')
        comm.len++;
    tokenizer->tokens[tokenizer->num_tokens++] = comm;
}

bool tokenize_obj_number(t_obj_tokenizer* tokenizer, float *fret, bool *is_int, t_obj_token* tok)
{
    int whole_part;
	int fract_part;
	int num_fract;
	int sign;

	ft_assert(ft_strchr("-0123456789", peek_obj_char(tokenizer)) != 0);
	*is_int = true;
	whole_part = 0;
	sign = 1;
	if (peek_obj_char(tokenizer) == '-') {
		sign = -1;
		consume_obj_char(tokenizer);
        tok->len++;
	}
	while (ft_strchr("0123456789", peek_obj_char(tokenizer)))
	{
		whole_part *= 10;
		whole_part += consume_obj_char(tokenizer) - '0';
        tok->len++;
	}
	if (peek_obj_char(tokenizer) == '.')
	{
		consume_obj_char(tokenizer);
        tok->len++;
		*is_int = false;
	}
	fract_part = 0;
	num_fract = 0;
	while (ft_strchr("0123456789", peek_obj_char(tokenizer)))
	{
		fract_part *= 10;
		fract_part += consume_obj_char(tokenizer) - '0';
        tok->len++;
		num_fract++;
	}
	*fret = whole_part * sign;
	*fret += fract_part / powf(10, num_fract);
	return (true);
}

bool tokenize_obj_tuple(t_obj_tokenizer* tokenizer)
{
    t_obj_token tok;
    init_tok(&tok, OBJ_TUPLE, tokenizer->curr_idx);
    bool is_int = true;
    float ret = 0.f;
    int i = 0;

    if (peek_obj_char(tokenizer) == '/')
    {
        consume_obj_char(tokenizer);
        tok.len++;
        tok.vals[i++] = 0;
    }
    
    while (peek_obj_char(tokenizer) != ' ' && peek_obj_char(tokenizer) != '\n' && chars_left(tokenizer))
    {
        if (!tokenize_obj_number(tokenizer, &ret, &is_int, &tok))
            return (false);
        if (!is_int)
        {
            tok.parsed_num = ret;
            tok.t = OBJ_NUMBER;
            tokenizer->tokens[tokenizer->num_tokens++] = tok;
            return (true);
        }
        if (i < 3)
            tok.vals[i++] = (int)ret;
        if (peek_obj_char(tokenizer) == '/'){
            consume_obj_char(tokenizer);
            tok.len++;
            if (peek_obj_char(tokenizer) == '/'){
                consume_obj_char(tokenizer);
                tok.len++;
                tok.vals[i++] = 0;
            }
        }
    }
    tokenizer->tokens[tokenizer->num_tokens++] = tok;
    return (true);
}

bool tokenize_word(t_obj_tokenizer* tokenizer)
{
    t_obj_token tok;
    init_tok(&tok, OBJ_IDENT, tokenizer->curr_idx);
    while (peek_obj_char(tokenizer) != ' ' && peek_obj_char(tokenizer) != '\n' && chars_left(tokenizer))
    {
        tok.len++;
        consume_obj_char(tokenizer);
    }
    tokenizer->tokens[tokenizer->num_tokens++] = tok;
    return (true);
}

void tokenize_newline(t_obj_tokenizer* tokenizer)
{
    t_obj_token tok;
    init_tok(&tok, OBJ_NEWLINE, tokenizer->curr_idx);
    tok.len++;
    tokenizer->curr_idx++;
    tokenizer->tokens[tokenizer->num_tokens++] = tok;
}

char* obj_type_conversion(unsigned int t)
{
    if (t == OBJ_NONE)
        return ("none");
    else if (t == OBJ_IDENT)
        return ("ident");
    else if (t == OBJ_NUMBER)
        return ("number");
    else if (t == OBJ_COMMENT)
        return ("comment");
    else if (t == OBJ_WHITESPACE)
        return ("whitespace");
    else if (t == OBJ_NEWLINE)
        return ("newline");
    else if (t == OBJ_TUPLE)
        return ("tuple");
    else
        return ("eof");
}

//add token array to function signature
int process_obj_file(char* filename, t_obj_tokenizer* tokenizer)
{
    t_dyn_str ret = {0};

    if (!dyn_str_read_file(filename, &ret))
        return 0;
    tokenizer->str = ret;

    while (chars_left(tokenizer))
    {
        if (peek_obj_char(tokenizer) == '#')
            tokenize_comment(tokenizer);
        else if (peek_obj_char(tokenizer) == '\n')
            tokenize_newline(tokenizer); 
        else if (peek_obj_char(tokenizer) == ' ')
            consume_obj_char(tokenizer);
        else if (peek_obj_char(tokenizer) == '-' || peek_obj_char(tokenizer) == '/' || ft_strchr("0123456789", peek_obj_char(tokenizer)))
        {
            if (!tokenize_obj_tuple(tokenizer))
                return (0);
        }
        else
            tokenize_word(tokenizer);
    }

    //PRINT STUFF
    printf("num tokens: %d\n", tokenizer->num_tokens);
    int i = -1;
    while (++i < tokenizer->num_tokens)
    {
        printf("[type: %s; %.*s]\n", obj_type_conversion(tokenizer->tokens[i].t), tokenizer->tokens[i].len, tokenizer->str.buff + tokenizer->tokens[i].start_idx);
        if (tokenizer->tokens[i].parsed_num != -99999.f)
           printf("num: %f\n", tokenizer->tokens[i].parsed_num);
        if (tokenizer->tokens[i].vals[0] != 0)
        {
            printf("vals: %d\n", tokenizer->tokens[i].vals[0]); 
            printf("vals: %d\n", tokenizer->tokens[i].vals[1]); 
            printf("vals: %d\n", tokenizer->tokens[i].vals[2]);
        }
    }
    return 1;
}
