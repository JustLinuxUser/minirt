/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_shape.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:52:15 by anddokhn          #+#    #+#             */
/*   Updated: 2025/01/14 00:34:37 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_SHAPE_H
# define VEC_SHAPE_H
# include <stdlib.h>
# include "../shapes.h"

typedef struct s_vec_shape
{
	size_t	cap;
	size_t	len;
	t_shape		*buff;
}	t_vec_shape;
int	vec_shape_init(t_vec_shape *ret, size_t size);
int	vec_shape_double(t_vec_shape *v);
int	vec_shape_push(t_vec_shape *v, t_shape el);
t_shape	vec_shape_pop(t_vec_shape *v);
t_shape	vec_shape_idx(t_vec_shape *v, size_t idx);
#endif
