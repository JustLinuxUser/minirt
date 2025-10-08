/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 16:11:28 by mhornero          #+#    #+#             */
/*   Updated: 2025/09/26 19:40:45 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "obj_tokenizer.h"

typedef struct s_obj_parser {
    size_t curr_token;
    t_obj_tokenizer tokenizer;
    t_vec_fvec3 vertices;
    t_vec_int faces;
} t_obj_parser;

int get_obj(char *filename, t_obj_parser *ret);
