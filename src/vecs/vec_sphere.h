/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_sphere.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anddokhn <anddokhn@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:52:15 by anddokhn          #+#    #+#             */
/*   Updated: 2025/01/14 00:34:37 by anddokhn         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VEC_SPHERE_H
# define VEC_SPHERE_H
# include <stdlib.h>
typedef int t_sphere;

typedef struct s_vec_sphere
{
	size_t	cap;
	size_t	len;
	t_sphere		*buff;
}	t_vec_sphere;
int	vec_sphere_init(t_vec_sphere *ret);
int	vec_sphere_double(t_vec_sphere *v);
int	vec_sphere_push(t_vec_sphere *v, t_sphere el);
t_sphere	vec_sphere_pop(t_vec_sphere *v);
t_sphere	vec_sphere_idx(t_vec_sphere *v, size_t idx);
#endif
