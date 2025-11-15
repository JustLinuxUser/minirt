/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_tokenizer.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhornero <mhornero@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 19:04:29 by mhornero          #+#    #+#             */
/*   Updated: 2025/11/02 18:48:01 by mhornero         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJ_TOKENIZER_H
# define OBJ_TOKENIZER_H

# include "../rt_file/rt_parser.h"
# include <stdio.h>
# include "obj_structs.h"
# include "../libft/generated/vec_obj_token.h"

typedef struct t_obj_tokenizer
{
	size_t			curr_idx;
	t_vec_obj_token	tokens;
	t_dyn_str		str;
}	t_obj_tokenizer;

int		tokenize_obj(char *filename, t_obj_tokenizer *tokenizer);
int		chars_left(t_obj_tokenizer *tokenizer);
void	init_tok(t_obj_token *tok, enum e_OBJ_TT t, int curr_idx);
char	consume_obj_char(t_obj_tokenizer *tokenizer);
char	peek_obj_char(t_obj_tokenizer *tokenizer);
char	*obj_type_conversion(unsigned int t);
bool	tokenize_obj_tuple(t_obj_tokenizer *tokenizer);
bool	tokenize_obj_number(t_obj_tokenizer *tokenizer,
			float *fret, bool *is_int, t_obj_token *tok);
void	tokenize_comment(t_obj_tokenizer *tokenizer);
bool	tokenize_word(t_obj_tokenizer *tokenizer);
void	tokenize_newline(t_obj_tokenizer *tokenizer);
#endif
