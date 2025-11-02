/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_structs.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 18:44:01 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 18:47:47 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

enum e_OBJ_TT
{
	OBJ_NONE = 0,
	OBJ_IDENT,
	OBJ_FLOAT,
	OBJ_COMMENT,
	OBJ_I_TUPLE,
	OBJ_WHITESPACE,
	OBJ_NEWLINE,
	OBJ_EOF,
};

typedef struct t_obj_token
{
	enum e_OBJ_TT	t;
	int				start_idx;
	int				len;
	float			parsed_num;
	int				vals[3];
	int				num_vals;
}	t_obj_token;
