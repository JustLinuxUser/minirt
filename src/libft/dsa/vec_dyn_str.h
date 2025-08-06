/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_dyn_str.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:52:15 by anddokhn          #+#    #+#             */
/*   Updated: 2024/10/20 16:51:13 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_DYN_STR_H
# define VEC_DYN_STR_H
# include <stdlib.h>
# include "dyn_str.h"

typedef struct s_vec_dyn_str
{
	size_t		cap;
	size_t		len;
	t_dyn_str	*buff;
}	t_vec_dyn_str;
int			vec_dyn_str_init(t_vec_dyn_str *ret);
int			vec_dyn_str_double(t_vec_dyn_str *v);
int			vec_dyn_str_push(t_vec_dyn_str *v, t_dyn_str el);
t_dyn_str	vec_dyn_str_pop(t_vec_dyn_str *v);
t_dyn_str	vec_dyn_str_idx(t_vec_dyn_str *v, size_t idx);
#endif
