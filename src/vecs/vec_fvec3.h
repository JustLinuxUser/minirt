/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_fvec3.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:52:15 by anddokhn          #+#    #+#             */
/*   Updated: 2025/01/14 00:34:37 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_FVEC3_H
# define VEC_FVEC3_H
# include <stdlib.h>
#include "../mymath.h"

typedef struct s_vec_fvec3
{
	size_t	cap;
	size_t	len;
	t_fvec3		*buff;
}	t_vec_fvec3;
int	vec_fvec3_init(t_vec_fvec3 *ret);
int	vec_fvec3_double(t_vec_fvec3 *v);
int	vec_fvec3_push(t_vec_fvec3 *v, t_fvec3 el);
t_fvec3	vec_fvec3_pop(t_vec_fvec3 *v);
t_fvec3	vec_fvec3_idx(t_vec_fvec3 *v, size_t idx);
#endif
