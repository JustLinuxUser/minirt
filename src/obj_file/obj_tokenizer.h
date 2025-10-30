/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_tokenizer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 19:04:29 by mhornero          #+#    #+#             */
/*   Updated: 2025/09/23 17:57:15 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../rt_file/rt_parser.h"
#include <stdio.h>


enum OBJ_TT {
    OBJ_NONE = 0,

	OBJ_IDENT,
	OBJ_FLOAT,
    OBJ_COMMENT,
    OBJ_I_TUPLE,
    OBJ_WHITESPACE,
    OBJ_NEWLINE,
	OBJ_EOF,
};

typedef struct t_obj_token {
    enum OBJ_TT t;
	int start_idx;
	int len;
    float parsed_num;
    int vals[3];
	int num_vals;
} t_obj_token;

#include "../libft/generated/vec_obj_token.h"

typedef struct t_obj_tokenizer {
    size_t curr_idx;
    t_vec_obj_token tokens;
    t_dyn_str str;
} t_obj_tokenizer;

int tokenize_obj(char* filename, t_obj_tokenizer* tokenizer);
