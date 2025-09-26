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
#include "../libft/dsa/vec_gen.h"

typedef struct s_obj_parser {
    size_t curr_token;
    t_obj_tokenizer tokenizer;
    t_vec_gen vertices;
    t_vec_gen vertex_normal_idx;
    t_vec_gen faces;
} t_obj_parser;

int get_obj(char *filename);
