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

#ifndef VEC_INT_H
# define VEC_INT_H
# include <stdlib.h>

typedef struct s_vec_int
{
	size_t	cap;
	size_t	len;
	int		*buff;
}	t_vec_int;
int	vec_int_init(t_vec_int *ret);
int	vec_int_double(t_vec_int *v);
int	vec_int_push(t_vec_int *v, int el);
int	vec_int_pop(t_vec_int *v);
int	vec_int_idx(t_vec_int *v, size_t idx);
#endif
