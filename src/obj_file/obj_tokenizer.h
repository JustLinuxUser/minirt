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
	OBJ_NUMBER,
    OBJ_COMMENT,
    OBJ_TUPLE,
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
} t_obj_token;

typedef struct t_obj_tokenizer {
    size_t curr_idx;
    t_obj_token *tokens;
    t_dyn_str str;
    int num_tokens;
} t_obj_tokenizer;

int process_obj_file(char* filename, t_obj_tokenizer* tokenizer);
