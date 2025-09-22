/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_tokenizer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:10:19 by mhornero          #+#    #+#             */
/*   Updated: 2025/09/22 20:38:56 by mhornero         ###   ########.fr       */
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
    comm.t = OBJ_COMMENT;
    comm.parsed_num = -99999.f;
    comm.start_idx = tokenizer->curr_idx;
    comm.len = 0;
    while (consume_obj_char(tokenizer) != '\n')
        comm.len++;
    tokenizer->tokens[tokenizer->num_tokens++] = comm;
}

void assign_token_type(t_obj_tokenizer* tokenizer, t_obj_token* tok)
{
    if (ft_isalpha(peek_obj_char(tokenizer)))
    {
        tok->parsed_num = -99999.f;
        tok->t = OBJ_IDENT;
    }
    else if (ft_isdigit(peek_obj_char(tokenizer)) || peek_obj_char(tokenizer) == '-')
        tok->t = OBJ_NUMBER;
    else
        tok->t = OBJ_EOF;
}

void tokenize_obj_number(t_obj_tokenizer* tokenizer, t_obj_token* tok)
{
    int whole_part;
	int fract_part;
	int num_fract;
	int sign;

	whole_part = 0;
	sign = 1;
    tok->parsed_num = 0.f;
	if (peek_obj_char(tokenizer) == '-') {
		sign = -1;
        tok->len++;
		consume_obj_char(tokenizer);
	}
	while (ft_strchr("0123456789", peek_obj_char(tokenizer)))
	{
        if (peek_obj_char(tokenizer) == '/')
        {
            tok->t = OBJ_TUPLE;
            return ;
        }
		whole_part *= 10;
		whole_part += consume_obj_char(tokenizer) - '0';
        tok->len++;
	}
	if (peek_obj_char(tokenizer) == '.')
    {
        tok->len++;
		consume_obj_char(tokenizer);
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
	tok->parsed_num = whole_part * sign;
	tok->parsed_num += fract_part / powf(10, num_fract);
    // printf("num: %f, %d, %f\n", whole_part, sign, fract_part);
}

void tokenize_obj_tuple(t_obj_tokenizer* tokenizer, t_obj_token* tok)
{
    tok->t = OBJ_TUPLE;
    while (peek_obj_char(tokenizer) != ' ' && chars_left(tokenizer))
    {
        tok->len++;
        consume_obj_char(tokenizer);
    }
}

void tokenize_word(t_obj_tokenizer* tokenizer)
{
    t_obj_token tok;
    assign_token_type(tokenizer, &tok);
    tok.start_idx = tokenizer->curr_idx;
    tok.len = 0;
    while (peek_obj_char(tokenizer) != ' ' && peek_obj_char(tokenizer) != '\n' && chars_left(tokenizer))
    {
        if (peek_obj_char(tokenizer) == '-' || ft_strchr("0123456789", peek_obj_char(tokenizer)))
        {
            tokenize_obj_number(tokenizer, &tok);
        }
        if (tok.t == OBJ_TUPLE)
        {
            tokenize_obj_tuple(tokenizer, &tok);
            break;
        }
        tok.len++;
        consume_obj_char(tokenizer);
    }
    tokenizer->tokens[tokenizer->num_tokens++] = tok;
}

void tokenize_newline(t_obj_tokenizer* tokenizer)
{
    t_obj_token tok;
    tok.t = OBJ_NEWLINE;
    tok.parsed_num = -99999.f;
    tok.start_idx = tokenizer->curr_idx;
    tok.len = 1;
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
int process_obj_file(char* filename)
{
    t_dyn_str ret = {0};
    t_obj_tokenizer tokenizer = {0};

    if (!dyn_str_read_file(filename, &ret))
        return 0;
    tokenizer.str = ret;
    // printf("ret: %s\n", tokenizer.str.buff);
    printf("pointer: %p\n", tokenizer.str.buff);
    while (chars_left(&tokenizer))
    {
        if (peek_obj_char(&tokenizer) == '#')
            tokenize_comment(&tokenizer);
        else if (peek_obj_char(&tokenizer) == '\n')
            tokenize_newline(&tokenizer); 
        else
            tokenize_word(&tokenizer);
    }
    printf("num tokens: %d\n", tokenizer.num_tokens);
    int i = -1;
    while (++i < tokenizer.num_tokens)
    {
        printf("start idx: %d\n", tokenizer.tokens[i].start_idx);
        printf("len: %d\n", tokenizer.tokens[i].len);
        printf("pointer: %p\n", tokenizer.str.buff);
        printf("[type: %s; %.*s]\n", obj_type_conversion(tokenizer.tokens[i].t), tokenizer.tokens[i].len, tokenizer.str.buff + tokenizer.tokens[i].start_idx);
        if (tokenizer.tokens[i].parsed_num != -99999.f)
           printf("num: %f\n", tokenizer.tokens[i].parsed_num);
    }
    return 1;
}
