/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_int.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:52:15 by anddokhn          #+#    #+#             */
/*   Updated: 2024/10/14 23:24:09 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_GEN_H
# define VEC_GEN_H
# include <stdint.h>
# include <stdlib.h>

typedef struct s_vec_gen
{
	size_t		el_size;
	size_t		cap;
	size_t		len;
	uint8_t		*buff;
}	t_vec_gen;

int		vec_gen_init(t_vec_gen *ret, size_t el_size, size_t init_alloc);
int		vec_int_double(t_vec_gen *v);
int		vec_int_push(t_vec_gen *v, void *el);
int		vec_int_pop(t_vec_gen *v, void *ret);
void	*vec_int_idx(t_vec_gen *v, size_t idx);
#endif
