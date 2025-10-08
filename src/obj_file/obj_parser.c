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
    if (parser->curr_token < (size_t)parser->tokenizer.num_tokens)
        return 1;
    return 0;
}

t_obj_token consume_obj_token(t_obj_parser* parser)
{
    return (parser->tokenizer.tokens[parser->curr_token++]);
}

t_obj_token peek_obj_token(t_obj_parser* parser)
{
    return (parser->tokenizer.tokens[parser->curr_token]); 
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
        else
            vert.z = num;
    }
    vec_fvec3_push(&parser->vertices, vert);
}

void parse_obj_faces(t_obj_parser *parser)
{
    while (peek_obj_token(parser).t == OBJ_TUPLE)
    {
        t_obj_token face = consume_obj_token(parser);
        int i = -1;
        while (++i < 3)
        {
            int ret = face.vals[i];
            vec_int_push(&parser->faces, ret);
        }
    }
}

int get_obj(char *filename)
{
    t_obj_parser parser = {0};
    parser.curr_token = 0;

    if (!process_obj_file(filename, &parser.tokenizer))
        return (0);

    vec_fvec3_init(&parser.vertices, 0);
    vec_int_init(&parser.faces, 0);

    while (tokens_left(&parser))
    {
        if (parser.tokenizer.str.buff[peek_obj_token(&parser).start_idx] == 'v')
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

    //PRINT
    for (int i = 0; i < 8; i++)
    {
        t_fvec3 ret = parser.vertices.buff[i]; 
        printf("here: %f %f %f\n", ret.x, ret.y, ret.z);
    }

    for (int i = 0; i < (int)parser.faces.len/3; i++)
    {

        int a = parser.faces.buff[i*3];
        int b = parser.faces.buff[i*3+1];
        int c = parser.faces.buff[i*3+2];  
        printf("here: %d %d %d\n", a, b, c);
    }
    
    return (1);
}
