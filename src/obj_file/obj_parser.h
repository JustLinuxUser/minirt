/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:11:28 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/01 18:04:41 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJ_PARSER_H
# define OBJ_PARSER_H

# include "obj_tokenizer.h"

typedef struct s_obj_parser
{
	size_t			curr_token;
	t_obj_tokenizer	tokenizer;
	t_vec_fvec3		vertices;
	t_vec_int		faces;
}	t_obj_parser;

int			parse_obj(char *filename, t_obj_parser *ret);
bool		tokens_left(t_obj_parser *parser);
t_obj_token	consume_obj_token(t_obj_parser *parser);
t_obj_token	peek_obj_token(t_obj_parser *parser);
#endif
