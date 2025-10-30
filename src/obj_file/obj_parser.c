/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 18:10:13 by mhornero          #+#    #+#             */
/*   Updated: 2025/09/26 20:42:01 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "obj_parser.h"
#include "../rt_utils.h"

bool tokens_left(t_obj_parser* parser)
{
    if (parser->curr_token < (size_t)parser->tokenizer.tokens.len)
        return 1;
    return 0;
}

t_obj_token consume_obj_token(t_obj_parser* parser)
{
    return (parser->tokenizer.tokens.buff[parser->curr_token++]);
}

t_obj_token peek_obj_token(t_obj_parser* parser)
{
    return (parser->tokenizer.tokens.buff[parser->curr_token]); 
}

bool parse_obj_vertices(t_obj_parser *parser)
{
    t_fvec3 vert = {0};
    int i = -1;
    while (peek_obj_token(parser).t == OBJ_FLOAT)
    {
        float num = consume_obj_token(parser).parsed_num;
		i++;
        if (i == 0)
            vert.x = num;
        else if (i == 1)
            vert.y = num;
        else if (i == 2)
            vert.z = num;
    }
	if (i != 2) return false;
    vec_fvec3_push(&parser->vertices, vert);
	return true;
}

bool parse_obj_faces(t_obj_parser *parser)
{
	int i = 0;

    while (peek_obj_token(parser).t == OBJ_I_TUPLE)
    {
        t_obj_token face = consume_obj_token(parser);
		if (face.num_vals == 0)
			return (false);
		// 1 indexe
		
		vec_int_push(&parser->faces, face.vals[0] - 1);

		// simple quad triangulation
		i++;
		if (i > 3) {
			int first = parser->faces.buff[parser->faces.len - i];
			int prev =  parser->faces.buff[parser->faces.len - 2];
			vec_int_push(&parser->faces, prev);
			vec_int_push(&parser->faces, first);
		}
    }
	if (i < 3)
	{
		return (false);
	}
	return (true);
}

void free_parser(t_obj_parser *parser) {
	free_zero(&parser->faces.buff);
	free_zero(&parser->vertices.buff);
	free_zero(&parser->tokenizer.tokens.buff);
	free_zero(&parser->tokenizer.str.buff);
}

int parse_obj(char *filename, t_obj_parser *ret)
{
    t_obj_parser parser = {0};
    parser.curr_token = 0;

    if (!tokenize_obj(filename, &parser.tokenizer))
        return (0);

    vec_fvec3_init(&parser.vertices, 0);
    vec_int_init(&parser.faces, 0);

    while (tokens_left(&parser))
    {
		
        if (str_slice_eq_str(&parser.tokenizer.str.buff[peek_obj_token(&parser).start_idx], peek_obj_token(&parser).len, "v"))
        {
            consume_obj_token(&parser);
            if (!parse_obj_vertices(&parser)) 
				return (free_parser(&parser), 0);
        }
        else if (parser.tokenizer.str.buff[peek_obj_token(&parser).start_idx] == 'f')
        {
            consume_obj_token(&parser);
            if (!parse_obj_faces(&parser))
				return (free_parser(&parser), 0);
        }
        consume_obj_token(&parser);
    }
	// we can't segfault later, right?
	for (size_t i = 0; i < parser.faces.len; i++) {
		if (parser.faces.buff[i] < 0 || (size_t)parser.faces.buff[i] >= parser.vertices.len) {
			return (free_parser(&parser), 0);
		}
	}

	free(parser.tokenizer.tokens.buff);
	free(parser.tokenizer.str.buff);
	*ret = parser;
    return (1);
}
