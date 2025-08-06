/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_triangle.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:52:15 by anddokhn          #+#    #+#             */
/*   Updated: 2025/01/14 00:34:37 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_TRIANGLE_H
# define VEC_TRIANGLE_H
# include <stdlib.h>

typedef struct s_triangle {
	int mesh_idx;
	int triangle_idx;
}	t_triangle;

typedef struct s_vec_triangle
{
	size_t	cap;
	size_t	len;
	t_triangle		*buff;
}	t_vec_triangle;
int	vec_triangle_init(t_vec_triangle *ret);
int	vec_triangle_double(t_vec_triangle *v);
int	vec_triangle_push(t_vec_triangle *v, t_triangle el);
t_triangle	vec_triangle_pop(t_vec_triangle *v);
t_triangle	vec_triangle_idx(t_vec_triangle *v, size_t idx);
#endif
