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

void parse_obj_vertices(t_obj_parser *parser)
{
    t_fvec3 vert = {0};
    int i = -1;
    while (peek_obj_token(parser).t == OBJ_NUMBER)
    {
        float num = consume_obj_token(parser).parsed_num;
        if (++i == 0)
            vert.x = num;
        else if (i == 1)
            vert.y = num;
        else if (i == 2)
            vert.z = num;
    }
    vec_fvec3_push(&parser->vertices, vert);
}

void parse_obj_faces(t_obj_parser *parser)
{
	int i = 0;

    while (peek_obj_token(parser).t == OBJ_TUPLE)
    {
        t_obj_token face = consume_obj_token(parser);
		// 1 indexed
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
}

int get_obj(char *filename, t_obj_parser *ret)
{
    t_obj_parser parser = {0};
    parser.curr_token = 0;

    if (!process_obj_file(filename, &parser.tokenizer))
        return (0);

    vec_fvec3_init(&parser.vertices, 0);
    vec_int_init(&parser.faces, 0);

    while (tokens_left(&parser))
    {
		
        if (str_slice_eq_str(&parser.tokenizer.str.buff[peek_obj_token(&parser).start_idx], peek_obj_token(&parser).len, "v"))
        {
            consume_obj_token(&parser);
            parse_obj_vertices(&parser);
        }
        else if (parser.tokenizer.str.buff[peek_obj_token(&parser).start_idx] == 'f')
        {
            consume_obj_token(&parser);
            parse_obj_faces(&parser);
        }
        consume_obj_token(&parser);
    }

	free(parser.tokenizer.tokens.buff);
    
	*ret = parser;
    return (1);
}
